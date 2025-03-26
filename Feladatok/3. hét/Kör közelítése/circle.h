#include <SDL2/SDL.h>

#ifndef CIRCLE_H
#define CIRCLE_H


/* A jobb klikkel lehet a köröket létrehozni, a ballal pedig mozgatni azokat */


/**
 * Data of a circle object
 */
typedef struct Circle
{
	double x;
	double y;
	double radius;
    SDL_Color color;
} Circle;

/**
 * Set the data of the circle
 */
void set_circle_data(Circle* circle, double x, double y, double radius, SDL_Color color);

/**
 * Calculate the area of the circle.
 */
double calc_circle_area(const Circle* circle);

/**
 * Draws the circle outline
 */
void draw_circle_outline(SDL_Renderer* renderer, int cx, int cy, int radius, int segments, int dash_ratio, SDL_Color color);

/**
 * Puts an "X" marker in the middle of the circle, if the cursor is inside it
 */
void draw_marker(SDL_Renderer* renderer, int x, int y, char type, SDL_Color color);




#endif // CIRCLE_H
