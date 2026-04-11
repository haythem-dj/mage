#include "player.h"
#include "vec2.h"

#include <math.h>

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

void player_init(Player* player, Vec2 position, Vec2 size)
{
    player->size = size;
    player->color = 0xEE4433FF;
    player->position = position;
    player->velocity = vec2_zero();
    player->acceleration = vec2_zero();
    player->on_ground = false;
    player->move_acceleration = 2000.0f;
    player->jump_acceleration = 700.0f;
    player->max_speed = 500.f;
    player->friction = 2.0f;
    player->gravity = 2600.0f;
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

void player_render(Player* player, void* renderer)
{
    SDL_FRect player_rect = {player->position.x, player->position.y, player->size.x, player->size.y};
    uint8_t alpha = (player->color >> 8 * 0) & 0xFF;
    uint8_t blue  = (player->color >> 8 * 1) & 0xFF;
    uint8_t green = (player->color >> 8 * 2) & 0xFF;
    uint8_t red   = (player->color >> 8 * 3) & 0xFF;
    SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
    SDL_RenderFillRect(renderer, &player_rect);
}
