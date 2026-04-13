#ifndef ASSET_MANAGER_H_
#define ASSET_MANAGER_H_

#include "h.h"
#include "stdio.h"

#include <SDL3/SDL.h>

typedef struct
{
    H_Hash_Table(SDL_Texture*, texture_t) textures;
    SDL_Renderer* renderer;
} AssetManager;

int asset_manager_init(AssetManager* am, SDL_Renderer* renderer);
SDL_Texture* asset_manager_get_texture(AssetManager* am, const char* name);
SDL_Texture* asset_manager_load_texture(AssetManager* am, const char* name);
int asset_manager_destroy(AssetManager* am);

#endif // ASSET_MANAGER_H_
