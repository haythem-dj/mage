#include "sprite.h"

Sprite sprite_load(AssetManager* am, const char* path)
{
    Sprite sprite = {0};

    SDL_Texture* tex = asset_manager_load_texture(am, path);
    sprite.texture = tex;

    SDL_GetTextureSize(tex, &sprite.src.w, &sprite.src.h);

    return sprite;
}

void sprite_render(Sprite sprite, SDL_Renderer* renderer, SDL_FRect dst, bool flipped)
{
    SDL_RenderTextureRotated(
        renderer, sprite.texture, &sprite.src, &dst, 0.0, NULL, flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}
