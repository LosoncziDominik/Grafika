#include "boid.h"
#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Boid Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    init_boids();

    int running = 1;
    SDL_Event event;
    //Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        //float deltaTime = (currentTime - lastTime) / 1000.0f;
        //lastTime = currentTime;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = 0;
        }

        update_boids();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_boids(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
