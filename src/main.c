#include <stdlib.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "vec2.h"

#define WIDTH 800
#define HEIGHT 800

typedef struct
{
    Vec2 position;
    Vec2 size;
    float speed;
} Player;

typedef struct
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    Player player;

    SDL_Time last_time;
} AppState;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    AppState* app_state = (AppState*)malloc(sizeof(AppState));

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed. Error: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(
            "mage", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE, &app_state->window, &app_state->renderer))
    {
        SDL_Log("SDL_CreateWindowAndRenderer failed. Error: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    app_state->player.position = vec2(100.f, 100.f);
    app_state->player.size = vec2(100.f, 100.f);
    app_state->player.speed = 500.0f;

    app_state->last_time = SDL_GetTicks();

    *appstate = app_state;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT || (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE))
        return SDL_APP_SUCCESS;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    AppState* app_state = (AppState*)appstate;

    SDL_Time current = SDL_GetTicks();
    float dt = (current - app_state->last_time) / 1000.0f;
    app_state->last_time = current;

    const bool* keyboard_state = SDL_GetKeyboardState(NULL);

    Vec2 direction = vec2(0.0f, 0.0f);

    if (keyboard_state[SDL_SCANCODE_W]) direction.y -= 1.0f;
    else if (keyboard_state[SDL_SCANCODE_S])
        direction.y = 1.0f;
    else
        direction.y = 0.0f;

    if (keyboard_state[SDL_SCANCODE_A]) direction.x = -1.0f;
    else if (keyboard_state[SDL_SCANCODE_D])
        direction.x = 1.0f;
    else
        direction.x = 0.0f;

    direction = vec2_norm(direction);
    // vec2_log(direction);

    app_state->player.position =
        vec2_add(app_state->player.position, vec2_scale(direction, app_state->player.speed * dt));

    SDL_FRect player_rect = {
        app_state->player.position.x, app_state->player.position.y, app_state->player.size.x, app_state->player.size.y};

    SDL_SetRenderDrawColor(app_state->renderer, 24, 24, 24, 255);
    SDL_RenderClear(app_state->renderer);
    SDL_SetRenderDrawColor(app_state->renderer, 24, 240, 24, 255);
    SDL_RenderRect(app_state->renderer, &player_rect);
    SDL_RenderPresent(app_state->renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    AppState* app_state = (AppState*)appstate;
    SDL_DestroyWindow(app_state->window);
    SDL_DestroyRenderer(app_state->renderer);
    free(app_state);
}
