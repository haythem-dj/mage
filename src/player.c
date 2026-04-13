#include "common.h"
#include "player.h"

#include <math.h>
#include <stdio.h>

static int clamp(int value, int min, int max) { return value < min ? min : value > max ? max : value; }
static float clampf(float value, float min, float max) { return value < min ? min : value > max ? max : value; }

static float lerp(float a, float b, float t) { return a + t * (b - a); }

static void player_init_sprites(Player* player, AssetManager* am)
{
    player->sprites[PLAYER_STATE_IDLE] = sprite_load(am, "res/player/idle.png");
    player->sprites[PLAYER_STATE_IDLE].src.w = 128.0f;
    player->sprites[PLAYER_STATE_IDLE].src.h = 64.0f;

    player->sprites[PLAYER_STATE_RUN] = sprite_load(am, "res/player/run.png");
    player->sprites[PLAYER_STATE_RUN].src.w = 128.0f;
    player->sprites[PLAYER_STATE_RUN].src.h = 64.0f;

    player->sprites[PLAYER_STATE_JUMP] = sprite_load(am, "res/player/jump.png");
    player->sprites[PLAYER_STATE_JUMP].src.w = 128.0f;
    player->sprites[PLAYER_STATE_JUMP].src.h = 64.0f;
}

static void player_init_animations(Player* player)
{
    player->animations[PLAYER_STATE_IDLE] =
        (Animation){.time = 0.0f, .frame = 0, .frame_count = 4, .frame_speed = 5, .loop = true};
    player->animations[PLAYER_STATE_RUN] =
        (Animation){.time = 0.0f, .frame = 0, .frame_count = 4, .frame_speed = 10, .loop = true};
    player->animations[PLAYER_STATE_JUMP] =
        (Animation){.time = 0.0f, .frame = 0, .frame_count = 4, .frame_speed = 7, .loop = true};
}

static void player_move(Player* player, float dt)
{
    const bool* keyboard_state = SDL_GetKeyboardState(NULL);

    float target_velocity = 0;

    if (keyboard_state[SDL_SCANCODE_D] || keyboard_state[SDL_SCANCODE_RIGHT]) target_velocity = player->move_speed;
    if (keyboard_state[SDL_SCANCODE_A] || keyboard_state[SDL_SCANCODE_LEFT]) target_velocity = -player->move_speed;
    if (keyboard_state[SDL_SCANCODE_SPACE] && player->on_ground)
    {
        player->velocity.y = -player->jump_acceleration;
        player->on_ground = false;
    }

    float friction = player->on_ground ? player->friction : player->friction * 0.1;

    player->velocity.x = lerp(player->velocity.x, target_velocity, friction * dt);
    if (fabsf(player->velocity.x - target_velocity) < 100.0f) player->velocity.x = target_velocity;
    player->velocity.y += player->gravity * dt;

    player->position = vec2_add(player->position, vec2_scale(player->velocity, dt));

    if (player->position.y >= 500.0f - player->size.y)
    {
        player->position.y = 500.0f - player->size.y;
        player->velocity.y = 0.0f;
        player->on_ground = true;
    }
}

static void player_animate(Player* player, float dt)
{
    Animation* anim = &player->animations[player->state];
    anim->time += dt * anim->frame_speed;
    uint32_t index = (size_t)anim->time;
    if (anim->loop) index = index % anim->frame_count;
    else
        index = clamp(index, 0, anim->frame_count);
    player->sprites[player->state].src.x = player->sprites[player->state].src.w * index;
}

int player_init(Player* player, GameState* gs)
{
    player->position = vec2_zero();
    player->velocity = vec2_zero();

    player->size = vec2(128.0f * 1.5, 64.0f * 1.5);

    player->on_ground = false;
    player->jump_acceleration = 700.0f;
    player->move_speed = 400.f;
    player->friction = 20.0f;
    player->gravity = 2600.0f;

    player->flip = false;

    player->state = PLAYER_STATE_IDLE;

    player_init_sprites(player, &gs->assets);
    player_init_animations(player);

    return 1;
}

void player_update(Player* player, GameState* gs, float dt)
{
    player_move(player, dt);

    PlayerState last_state = player->state;

    if (player->on_ground)
    {
        if (player->velocity.x > 0)
        {
            player->state = PLAYER_STATE_RUN;
            player->flip = false;
        }
        else if (player->velocity.x < 0)
        {
            player->state = PLAYER_STATE_RUN;
            player->flip = true;
        }
        else
            player->state = PLAYER_STATE_IDLE;
    }
    else
        player->state = PLAYER_STATE_JUMP;

    if (last_state != player->state) player->animations[player->state].time = 0.0f;

    player_animate(player, dt);
}

void player_render(Player* player, GameState* gs, SDL_Renderer* renderer)
{
    SDL_FRect player_rect = {player->position.x, player->position.y, player->size.x, player->size.y};

    Sprite* active_sprite = &player->sprites[player->state];

    sprite_render(*active_sprite, renderer, player_rect, player->flip);
}
