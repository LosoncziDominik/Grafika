#ifndef BOID_H
#define BOID_H

#include <SDL2/SDL.h>

#define NUM_BOIDS 300
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 700
#define MAX_SPEED 3.0
#define MIN_SPEED 1.0
#define VIEW_RADIUS 50.0
#define SEPARATION_RADIUS 15.0
#define COHESION_WEIGHT 0.05
#define ALIGNMENT_WEIGHT 0.04
#define SEPARATION_WEIGHT 0.03
#define TURNING_RATE 0.05
#define SPEED_SMOOTHING 0.5
#define ROTATION_LIMIT 0.2


/**
 * Boid position and speed
 */
typedef struct {
    float x, y;
    float vx, vy;
} Boid;

extern Boid boids[NUM_BOIDS];

/**
 * Move the boid to the given position
 */
void init_boids();

/**
 * Updating the boid's behavior
 */
void update_boids();

/**
 * Rendering the screen
 */
void render_boids(SDL_Renderer* renderer);

#endif
