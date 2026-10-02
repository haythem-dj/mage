#include <stdio.h>
#include <stdlib.h>
#if 0
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "asset_manager.h"
#include "common.h"
#include "game_state.h"
#include "player.h"
#include "vec2.h"

#define INIT_WIDTH 600
#define INIT_HEIGHT 600

typedef struct
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    int width;
    int height;

    bool is_fullscreen;

    uint64_t last_time;

    GameState gs;

    Player player;
} AppState;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    AppState* app_state = (AppState*)malloc(sizeof(AppState));
    if (!app_state)
    {
        ERROR("malloc faile. Error: Could not allocate memory for AppState.");
        return SDL_APP_FAILURE;
    }

    app_state->width = INIT_WIDTH;
    app_state->height = INIT_HEIGHT;
    app_state->is_fullscreen = false;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        ERROR("SDL_Init failed. Error: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_WindowFlags flags =
        app_state->is_fullscreen ? SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE;

    if (!SDL_CreateWindowAndRenderer(
            "mage", app_state->width, app_state->height, flags, &app_state->window, &app_state->renderer))
    {
        ERROR("SDL_CreateWindowAndRenderer failed. Error: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    asset_manager_init(&app_state->gs.assets, app_state->renderer);
    if (player_init(&app_state->player, &app_state->gs) == 0)
    {
        ERROR("player_init failed.");
        return SDL_APP_FAILURE;
    }

    app_state->last_time = SDL_GetTicks();

    *appstate = app_state;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    AppState* app_state = (AppState*)appstate;

    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
        break;

    case SDL_EVENT_KEY_DOWN:
    {
        switch (event->key.scancode)
        {
        case SDL_SCANCODE_ESCAPE:
            return SDL_APP_SUCCESS;
            break;

        case SDL_SCANCODE_F11:
        {
            app_state->is_fullscreen = !app_state->is_fullscreen;
            SDL_SetWindowFullscreen(app_state->window, app_state->is_fullscreen);
            break;
        }

        default:
            break;
        }

        break;
    }

    case SDL_EVENT_WINDOW_RESIZED:
    {
        SDL_GetWindowSize(app_state->window, &app_state->width, &app_state->height);
        break;
    }

    default:
        break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    AppState* app_state = (AppState*)appstate;

    SDL_Time current = SDL_GetTicks();
    float dt = (current - app_state->last_time) / 1000.0f;
    app_state->last_time = current;

    player_update(&app_state->player, &app_state->gs, dt);

    SDL_SetRenderDrawColor(app_state->renderer, 24, 24, 24, 255);
    SDL_RenderClear(app_state->renderer);

    // player
    player_render(&app_state->player, &app_state->gs, app_state->renderer);

    // ground
    SDL_FRect ground_rect = {0.0f, 500.0f, app_state->width, 100.0f};
    SDL_SetRenderDrawColor(app_state->renderer, 0x24, 0xee, 0x24, 0xff);
    SDL_RenderFillRect(app_state->renderer, &ground_rect);
    SDL_SetRenderDrawColor(app_state->renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderRect(app_state->renderer, &ground_rect);

    SDL_RenderPresent(app_state->renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    AppState* app_state = (AppState*)appstate;

    SDL_DestroyRenderer(app_state->renderer);
    SDL_DestroyWindow(app_state->window);

    SDL_Quit();

    free(app_state);
}
#else

#include "ecs.h"

void transform_system(Transform* transform, void* user_data)
{
    Vec2 pos = transform->position;
    Vec2 sca = transform->scale;
    float rot = transform->rotation;

    printf("pos: (%f, %f), scale: (%f, %f), rot: %f\n", pos.x, pos.y, sca.x, sca.y, rot);
    transform->position.x += 0.01f;
}

int main(void)
{
    ECS ecs = {0};
    ecs_init(&ecs);

    // ecs_set_transform_system(&ecs, transform_system);

    EntityID e = ecs_create_entity(&ecs);
    EntityID e2 = ecs_create_entity(&ecs);

    Transform tr = {(Vec2){4.7f, -0.4f}, (Vec2){1.0f, 1.0f}, 0.0f};
    ecs_add_transform(&ecs, e, tr);
    ecs_add_transform(&ecs, e2, tr);
    ecs_remove_transform(&ecs, e);

    ecs_add_component(ecs, e, Transform, tr);

    ecs_destroy_entity(&ecs, e);
    ecs_destroy_entity(&ecs, e2);

    ecs_shutdown(&ecs);

    return 0;
}

#endif
