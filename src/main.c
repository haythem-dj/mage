#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>

#include "da.h"

#define WIDTH 800
#define HEIGHT 800

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        perror("SDL_Init failed");
        exit(EXIT_FAILURE);
    }

    SDL_Window* window = SDL_CreateWindow("mage", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    {
        perror("SDL_CreateWindow failed");
        exit(EXIT_FAILURE);
    }

    printf("Hello, World!\n");

    return 0;
}
