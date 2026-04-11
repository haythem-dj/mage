#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL3/SDL.h>
#include <stdint.h>

#include "vec2.h"

typedef struct
{
    SDL_Renderer* renderer;

    Vec2 size;
    uint32_t color;
    SDL_Texture* texture;

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

int player_init(Player* player, SDL_Renderer* renderer, Vec2 position, Vec2 size);
void player_update(Player* player, float dt);
void player_render(Player* player);

#endif // PLAYER_H_
