#include <stdlib.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "vec2.h"

#define INIT_WIDTH 600
#define INIT_HEIGHT 600

typedef struct
{
    Vec2 size;
    Vec2 position;
    Vec2 velocity;

    SDL_Color color;
    
    float speed;
    float gravity;
} Player;

void player_init(Player* player, Vec2 position, Vec2 size)
{
    player->size = size;
    player->position = position;
    player->velocity = vec2_zero();
    player->color = (SDL_Color){24, 240, 24, 255};
    player->speed = 500.0f;
    player->gravity = 100.0f;
}

void player_update(Player* player, float dt)
{
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

    player->position = vec2_add(player->position, vec2_scale(direction, player->speed * dt));
}

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

    if (!SDL_CreateWindowAndRenderer(
            "mage", app_state->width, app_state->height, SDL_WINDOW_RESIZABLE, &app_state->window, &app_state->renderer))
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

    SDL_FRect player_rect = {
        app_state->player.position.x, app_state->player.position.y, app_state->player.size.x, app_state->player.size.y};

    SDL_SetRenderDrawColor(app_state->renderer, 24, 24, 24, 255);
    SDL_RenderClear(app_state->renderer);

    SDL_SetRenderDrawColor(app_state->renderer, 24, 240, 24, 255);
    SDL_RenderFillRect(app_state->renderer, &player_rect);
    
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
