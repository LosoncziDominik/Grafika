#ifndef BOIDS_H
#define BOIDS_H 
#include <stdbool.h>
#include <stdio.h>
#include "include/glad/glad.h"
#include "include/KHR/khrplatform.h"
#include <math.h>
#include <SDL2/SDL.h>


#ifndef CALLBACK
#define CALLBACK
#endif
#include <GL/glu.h>

#define NUM_BOIDS 100
#define SPACE_WIDTH 250
#define SPACE_HEIGHT 250
#define SPACE_DEPTH 250
#define MAX_SPEED 0.5
#define MIN_SPEED 0.05
#define VIEW_RADIUS 15.0
#define SEPARATION_RADIUS 5.0
#define COHESION_WEIGHT 0.3f
#define ALIGNMENT_WEIGHT 0.04f
#define SEPARATION_WEIGHT 0.8f
#define TURNING_RATE 0.007f
#define SPEED_SMOOTHING 0.002
#define PLAYER_SPEED 5.0
#define MAX_BLOOD 1000
#define MAX_OPTIONS 5
#define M_PI 3.14159265358979323846
#define MAX_GROUPS 200
#define GROUP_RADIUS 20.0
#define MAX_VERTICES 10000
#define MAX_FACES 100000

/**
 * Data of Boid
 */
typedef struct {
    float x, y, z;
    float vx, vy ,vz;
    bool alive;
    float targetSpeed;
    float speeds;
    float targetspeeds;
    float r, g, b;
    float frame_timer;
    int frame_index;
} Boid;

/**
 * Data of Player
 */
typedef struct {
    float x, y, z;
    float vx, vy, vz;
    float angle;
    int counter;
} Player;

typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    int v1, v2, v3;
} Face;

typedef struct{
    int x, y, z;
    float R, r;
}Torus;

typedef struct {
    float x, y, z;
}Vec3;

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
void update_boids();


/**
 * If an object passes the screen length it will come out of the other side
 */
void wrap_boid(Boid* b);


/**
 * Counts how many boids have been killed
 */
int score_counter();


/**
 * Getter function for the player's score
 */
int get_player_score();


void render_boids();

void draw_plane(float x, float y, float z);

void draw_test_cube();

void draw_bounds();

void assign_boid_groups();

void whatspeed();

void load_obj(const char* filename, int index);

void render_model_instance(float x, float y, float z, float angle, int index);

void create_donut_shape();

Vec3 vec3_sub(Vec3 a, Vec3 b);

Vec3 vec3_add(Vec3 a, Vec3 b);

Vec3 vec3_mul(Vec3 v, float s);

float vec3_dot(Vec3 a, Vec3 b);

float vec3_length(Vec3 v);

Vec3 vec3_normalize(Vec3 v);

int get_torus_x(int index);

int get_torus_y(int index);

int get_torus_z(int index);

Vec3 avoid_torus(Vec3 boidPos, Vec3 boidVel, Vec3 torusCenter, float R, float r);

Vec3 get_center(int index);

Vec3 get_boid_vel(int index);

void init_torus();

void draw_torus_debug_wire(int index);

#endif