#include <stdlib.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

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

    Player player;
} AppState;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    AppState* app_state = (AppState*)malloc(sizeof(AppState));
    if (!app_state)
    {
        SDL_Log("malloc faile. Error: Could not allocate memory for AppState.");
        return SDL_APP_FAILURE;
    }

    app_state->width = INIT_WIDTH;
    app_state->height = INIT_HEIGHT;
    app_state->is_fullscreen = false;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed. Error: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("mage", app_state->width, app_state->height, SDL_WINDOW_RESIZABLE,
            &app_state->window, &app_state->renderer))
    {
        SDL_Log("SDL_CreateWindowAndRenderer failed. Error: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    player_init(&app_state->player, vec2_zero(), (Vec2){100.0f, 100.0f});

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
        SDL_Log("%d", app_state->width);
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

    player_update(&app_state->player, dt);

    SDL_SetRenderDrawColor(app_state->renderer, 24, 24, 24, 255);
    SDL_RenderClear(app_state->renderer);

    // player
    player_render(&app_state->player, app_state->renderer);

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
