#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL3/SDL.h>
#include <stdint.h>

#include "asset_manager.h"
#include "game_state.h"
#include "sprite.h"
#include "vec2.h"

typedef struct
{
    size_t frame;
    float frame_speed;
    size_t frame_count;
    float time;
    bool loop;
} Animation;

typedef enum
{
    PLAYER_STATE_IDLE,
    PLAYER_STATE_RUN,
    PLAYER_STATE_JUMP,
    PLAYER_STATE_COUNT
} PlayerState;

typedef struct
{
    Vec2 size;

    Vec2 acceleration;
    Vec2 velocity;
    Vec2 position;

    bool on_ground;

    float jump_acceleration;
    float move_speed;
    float friction;
    float gravity;

    bool flip;

    PlayerState state;

    Sprite sprites[PLAYER_STATE_COUNT];
    Animation animations[PLAYER_STATE_COUNT];
} Player;

int player_init(Player* player, GameState* gs);
void player_update(Player* player, GameState* gs, float dt);
void player_render(Player* player, GameState* gs, SDL_Renderer* renderer);

#endif // PLAYER_H_
