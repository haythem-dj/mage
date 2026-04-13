#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL3/SDL.h>

#include "asset_manager.h"

typedef struct
{
    SDL_Texture* texture;
    SDL_FRect src;
} Sprite;

Sprite sprite_load(AssetManager* am, const char* path);
void sprite_render(Sprite sprite, SDL_Renderer* renderer, SDL_FRect dst, bool flipped);

#endif // SPRITE_H_
