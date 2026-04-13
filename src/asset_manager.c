#include "asset_manager.h"

#include "common.h"

int asset_manager_init(AssetManager* am, SDL_Renderer* renderer)
{
    h_ht_init(am->textures);
    am->renderer = renderer;
    return 1;
}

SDL_Texture* asset_manager_get_texture(AssetManager* am, const char* path)
{
    SDL_Texture** texture = h_ht_get(am->textures, path);
    if (!texture) return NULL;
    return *texture;
}

SDL_Texture* asset_manager_load_texture(AssetManager* am, const char* path)
{
    SDL_Texture* texture = asset_manager_get_texture(am, path);
    if (texture != NULL) return texture;

    SDL_Surface* surface = SDL_LoadSurface(path);
    if (!surface)
    {
        ERROR("SDL_LoadSurface failed, Error: %s", SDL_GetError());
        return NULL;
    }

    texture = SDL_CreateTextureFromSurface(am->renderer, surface);
    SDL_DestroySurface(surface);

    if (!texture)
    {
        ERROR("SDL_CreateTextureFromSurface failed. Error: %s", SDL_GetError());
        return NULL;
    }

    h_ht_insert(am->textures, path, texture);
    return texture;
}

int asset_manager_destroy(AssetManager* am)
{
    for (size_t i = 0; i < H_HT_TABLE_SIZE; i++)
    {
        __typeof__(*(am->textures)) e = am->textures[i];
        while (e)
        {
            SDL_DestroyTexture(e->value);
            e = e->next;
        }
    }
    h_ht_free(am->textures);
    am->renderer = NULL;
    return 1;
}
