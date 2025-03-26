#include "circle.h"
#include <math.h>
#include <SDL2/SDL_stdinc.h>


void draw_circle_outline(SDL_Renderer* renderer, int cx, int cy, int radius, int segments, int dash_ratio, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    double angle_step = 2 * 3.14 / segments;
    for (int i = 0; i < segments; i += dash_ratio * 2) {
        for (int j = 0; j < dash_ratio && (i + j + 1) < segments; ++j) {
            double angle1 = (i + j) * angle_step;
            double angle2 = (i + j + 1) * angle_step;

            int x1 = cx + (int)(radius * cos(angle1));
            int y1 = cy + (int)(radius * sin(angle1));
            int x2 = cx + (int)(radius * cos(angle2));
            int y2 = cy + (int)(radius * sin(angle2));

            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }
    }
}

void draw_marker(SDL_Renderer* renderer, int x, int y, char type, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    int size = 10;

    if (type == '+') {
        SDL_RenderDrawLine(renderer, x - size, y, x + size, y);
        SDL_RenderDrawLine(renderer, x, y - size, x, y + size);
    } else if (type == 'x') {
        SDL_RenderDrawLine(renderer, x - size, y - size, x + size, y + size);
        SDL_RenderDrawLine(renderer, x - size, y + size, x + size, y - size);
    }
}

void set_circle_data(Circle* circle, double x, double y, double radius, SDL_Color color)
{
	circle->x = x;
	circle->y = y;
	if (radius > 0.0) {
		circle->radius = radius;
	} else {
		circle->radius = NAN;
	}

	circle->color = color;
     
}

double calc_circle_area(const Circle* circle)
{
	double area = circle->radius * circle->radius * 3.14;
	return area;
}



