#define SDL_MAIN_HANDLED
#include "circle.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#define MAX_CIRCLES 10

typedef struct {
    int cx, cy;
    int radius;
    SDL_Color color;
} Circles;

SDL_Color colors[6] = {
    {255, 0, 0, 255},   // Piros
    {0, 255, 0, 255},   // Zöld
    {0, 0, 255, 255},   // Kék
    {255, 255, 0, 255}, // Sárga
    {255, 0, 255, 255}, // Lila
    {0, 255, 255, 255}  // Cián
};


int main(int argc, char* argv[])
{
    srand(time(NULL));

    int dragging = 0;
    int dragged_index = -1;


    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Szaggatott vonalas körök",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Circles circles[MAX_CIRCLES];
    int circle_count = 0;

    int click_state = 0;
    int center_x, center_y;

    SDL_Event event;
    int running = 1;

    while (running)
    {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int x = event.button.x;
                int y = event.button.y;
        
                for (int i = circle_count - 1; i >= 0; --i) {
                    int dx = x - circles[i].cx;
                    int dy = y - circles[i].cy;
                    int dist_squared = dx * dx + dy * dy;
        
                    if (dist_squared <= circles[i].radius * circles[i].radius) {
                        dragging = 1;
                        dragged_index = i;
                        break;
                    }
                }
            }
            else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                dragging = 0;
                dragged_index = -1;
            }
            else if (event.type == SDL_MOUSEMOTION && dragging) {
                int mx = event.motion.x;
                int my = event.motion.y;
        
                circles[dragged_index].cx = mx;
                circles[dragged_index].cy = my;
            }
        
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {
                int x = event.button.x;
                int y = event.button.y;
        
                if (click_state == 0) {
                    center_x = x;
                    center_y = y;
                    click_state = 1;
                } else if (click_state == 1) {
                    int dx = x - center_x;
                    int dy = y - center_y;
                    int radius = (int)sqrt(dx * dx + dy * dy);
        
                    if (circle_count < MAX_CIRCLES) {
                        circles[circle_count].cx = center_x;
                        circles[circle_count].cy = center_y;
                        circles[circle_count].radius = radius;
                        circles[circle_count].color = colors[rand() % 6];
                        circle_count++;
                    }
        
                    click_state = 0;
                }
            }
        }
        

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        int mx, my;
        SDL_GetMouseState(&mx, &my);

        for (int i = 0; i < circle_count; ++i) {
            draw_circle_outline(renderer,
            circles[i].cx, circles[i].cy,
            circles[i].radius, 200, 4,
            circles[i].color);
            int dx = mx - circles[i].cx;
            int dy = my - circles[i].cy;
            int dist_squared = dx * dx + dy * dy;

            if (dist_squared <= circles[i].radius * circles[i].radius) {
                draw_marker(renderer, circles[i].cx, circles[i].cy, '+', circles[i].color);
            }
        }

        SDL_RenderPresent(renderer);

    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
