#include "player.h"
#include "vec2.h"

#include <math.h>
#include <stdio.h>

#define ERROR(fmt, ...) fprintf(stderr, "[ERROR] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

static float clamp(float value, float min, float max) { return value < min ? min : value > max ? max : value; }

static void player_move(Player* player)
{
    const bool* keyboard_state = SDL_GetKeyboardState(NULL);
    if (keyboard_state[SDL_SCANCODE_D] || keyboard_state[SDL_SCANCODE_RIGHT])
        player->acceleration.x = player->move_acceleration;
    if (keyboard_state[SDL_SCANCODE_A] || keyboard_state[SDL_SCANCODE_LEFT])
        player->acceleration.x = -player->move_acceleration;
    if (keyboard_state[SDL_SCANCODE_SPACE] && player->on_ground)
    {
        player->velocity.y = -player->jump_acceleration;
        player->on_ground = false;
    }
}

int player_init(Player* player, SDL_Renderer* renderer, Vec2 position, Vec2 size)
{
    player->renderer = renderer;

    player->size = vec2(128.0f, 64.0f);

    player->position = position;
    player->velocity = vec2_zero();
    player->acceleration = vec2_zero();
    player->on_ground = false;
    player->move_acceleration = 2000.0f;
    player->jump_acceleration = 700.0f;
    player->max_speed = 500.f;
    player->friction = 2.0f;
    player->gravity = 2600.0f;

    SDL_Surface* surface = SDL_LoadSurface("res/player-idle-1.png");
    if (!surface)
    {
        ERROR("SDL_LoadSurface failed. Error: %s", SDL_GetError());
        return 0;
    }

    player->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    if (!player->texture)
    {
        ERROR("SDL_CreateTextureFromSurface failed. Error: %s", SDL_GetError());
        return 0;
    }
}

void player_update(Player* player, float dt)
{
    player->acceleration = vec2(0.0f, player->gravity);
    player_move(player);

    if (player->on_ground) player->acceleration.x -= player->friction * player->velocity.x;
    player->velocity = vec2_add(player->velocity, vec2_scale(player->acceleration, dt));
    if (player->velocity.y > 0) player->acceleration.y *= 1.5f;
    player->velocity.x = clamp(player->velocity.x, -player->max_speed, player->max_speed);
    player->position = vec2_add(player->position, vec2_scale(player->velocity, dt));

    if (player->position.y >= 500.0f - player->size.y)
    {
        player->position.y = 500.0f - player->size.y;
        player->velocity.y = 0.0f;
        player->on_ground = true;
    }
}

void player_render(Player* player)
{
    SDL_FRect player_rect = {player->position.x, player->position.y, player->size.x, player->size.y};
    SDL_RenderTexture(player->renderer, player->texture, NULL, &player_rect);
}
