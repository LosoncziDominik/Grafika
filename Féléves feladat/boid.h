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
 * Data of Boid
 */
typedef struct {
    float x, y;
    float vx, vy;
    bool alive;
} Boid;

/**
 * Data of Player
 */
typedef struct {
    float x, y;
    float vx, vy;
    float angle;
    int counter;
} Player;

/**
 * Data of Blood
 */
typedef struct{
    float x, y;
    bool active;
} Blood;

/**
 * Move the boid to the given position
 */
void init_boids();

/**
 * Move the player to the starting position
 */
void init_player();

/**
 * Updates the player's behaveior
 */
void update_player();

/**
 * Updates the boid's behavior
 */
void update_boids(SDL_Renderer* renderer);

/**
 * Renders the boids
 */
void render_boids(SDL_Renderer* renderer);

/**
 * Renders the player
 */
void render_player(SDL_Renderer* renderer);

/**
 * If an object passes the screen length it will come out of the other side
 */
void warp(float* x, float* y);

/**
 * Draws the player's score
 */
void draw_score(SDL_Renderer* renderer, const char* score, int x, int y, TTF_Font* font, SDL_Color color);

/**
 * Counts how many boids have been killed
 */
int score_counter();

/**
 * Renders the blood droplets
 */
void render_blood(SDL_Renderer* renderer);

/**
 * Getter function for the player's score
 */
int get_player_score();

#endif
