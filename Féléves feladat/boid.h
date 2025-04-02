#ifndef BOID_H
#define BOID_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

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
#define TURNING_RATE 0.1
#define SPEED_SMOOTHING 0.5
#define PLAYER_SPEED 5.0
#define MAX_BLOOD 1000


/**
 * Boid position and speed
 */
typedef struct {
    float x, y;
    float vx, vy;
    bool alive;
} Boid;

typedef struct {
    float x, y;
    float vx, vy;
    float angle;
    int counter;
} Player;

typedef struct{
    float x, y;
    SDL_Color color;
    bool active;
} Blood;

extern SDL_Event event;

extern Player player;

extern Boid boids[NUM_BOIDS];

/**
 * Move the boid to the given position
 */
void init_boids();

void init_player();

void update_player();

/**
 * Updating the boid's behavior
 */
void update_boids(SDL_Renderer* renderer);

/**
 * Rendering the screen
 */
void render_boids(SDL_Renderer* renderer);

void render_player(SDL_Renderer* renderer);

void warp(float* x, float* y);

void draw_score(SDL_Renderer* renderer, const char* score, int x, int y, TTF_Font* font, SDL_Color color);

int scoreCounter();

void render_blood(SDL_Renderer* renderer);

#endif
