#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PALETTE_WIDTH 100
#define MAX_LINE_COUNT 10
#define COLOR_COUNT 6

typedef struct {
    int x1, y1, x2, y2;
    SDL_Color color;
} Line;

SDL_Color colors[COLOR_COUNT] = {
    {255, 0, 0, 255},   // Piros
    {0, 255, 0, 255},   // Zöld
    {0, 0, 255, 255},   // Kék
    {255, 255, 0, 255}, // Sárga
    {255, 0, 255, 255}, // Lila
    {0, 255, 255, 255}  // Cián
};

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Színpaletta + Szakasz Rajzoló",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Line lines[MAX_LINE_COUNT];
    int line_count = 0;
    int click_state = 0;
    int temp_x, temp_y;
    SDL_Color selected_color = {255, 255, 255, 255};

    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;

                if (mx < PALETTE_WIDTH) {
                    int color_index = my / (WINDOW_HEIGHT / COLOR_COUNT);
                    if (color_index >= 0 && color_index < COLOR_COUNT) {
                        selected_color = colors[color_index];
                    }
                } 
                else {
                    if (click_state == 0) {
                        temp_x = mx;
                        temp_y = my;
                        click_state = 1;
                    } else if (click_state == 1 && line_count < MAX_LINE_COUNT) {
                        lines[line_count].x1 = temp_x;
                        lines[line_count].y1 = temp_y;
                        lines[line_count].x2 = mx;
                        lines[line_count].y2 = my;
                        lines[line_count].color = selected_color;
                        line_count++;
                        click_state = 0;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < COLOR_COUNT; i++) {
            SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, 255);
            SDL_Rect color_box = {0, i * (WINDOW_HEIGHT / COLOR_COUNT), PALETTE_WIDTH, WINDOW_HEIGHT / COLOR_COUNT};
            SDL_RenderFillRect(renderer, &color_box);
        }

        for (int i = 0; i < line_count; i++) {
            SDL_SetRenderDrawColor(renderer, lines[i].color.r, lines[i].color.g, lines[i].color.b, 255);
            SDL_RenderDrawLine(renderer, lines[i].x1, lines[i].y1, lines[i].x2, lines[i].y2);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
