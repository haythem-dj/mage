#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL3/SDL.h>
#include <stdint.h>

#include "vec2.h"

typedef struct
{
    Vec2 size;
    uint32_t color;

    Vec2 acceleration;
    Vec2 velocity;
    Vec2 position;

    bool on_ground;

    float move_acceleration;
    float jump_acceleration;
    float max_speed;
    float friction;

    float gravity;
} Player;

void player_init(Player* player, Vec2 position, Vec2 size);
void player_update(Player* player, float dt);
void player_render(Player* player, void* renderer);

#endif // PLAYER_H_
