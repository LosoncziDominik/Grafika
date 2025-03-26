#include "boid.h"
#include <math.h>
#include <stdlib.h>

Boid boids[NUM_BOIDS];

void init_boids() {
    for (int i = 0; i < NUM_BOIDS; i++) {
        boids[i].x = rand() % SCREEN_WIDTH;
        boids[i].y = rand() % SCREEN_HEIGHT;
        boids[i].vx = ((rand() % 200) / 100.0f - 1) * MAX_SPEED;
        boids[i].vy = ((rand() % 200) / 100.0f - 1) * MAX_SPEED;
    }
}

void update_boids() {
    for (int i = 0; i < NUM_BOIDS; i++) {
        float avgX = 0, avgY = 0;
        float avgVX = 0, avgVY = 0;
        float sepX = 0, sepY = 0;
        int count = 0;

        for (int j = 0; j < NUM_BOIDS; j++) {
            if (i != j) {
                float dx = boids[j].x - boids[i].x;
                float dy = boids[j].y - boids[i].y;
                float dist = sqrt(dx * dx + dy * dy);

                if (dist < VIEW_RADIUS) {
                    avgX += boids[j].x;
                    avgY += boids[j].y;
                    avgVX += boids[j].vx;
                    avgVY += boids[j].vy;
                    count++;
                }

                if (dist < SEPARATION_RADIUS) {
                    sepX -= dx;
                    sepY -= dy;
                }
            }
        }

        if (count > 0) {
            avgX /= count;
            avgY /= count;
            avgVX /= count;
            avgVY /= count;

            float targetVX = (avgX - boids[i].x) * COHESION_WEIGHT;
            float targetVY = (avgY - boids[i].y) * COHESION_WEIGHT;

            boids[i].vx += (targetVX - boids[i].vx) * TURNING_RATE;
            boids[i].vy += (targetVY - boids[i].vy) * TURNING_RATE;

            boids[i].vx += (avgVX - boids[i].vx) * ALIGNMENT_WEIGHT;
            boids[i].vy += (avgVY - boids[i].vy) * ALIGNMENT_WEIGHT;
        }

        boids[i].vx += sepX * SEPARATION_WEIGHT;
        boids[i].vy += sepY * SEPARATION_WEIGHT;

        float speed = sqrt(boids[i].vx * boids[i].vx + boids[i].vy * boids[i].vy);
        float targetSpeed = MAX_SPEED * 0.7 + (rand() % 30) * 0.01;

        if (speed < targetSpeed) {
            boids[i].vx *= (1 + SPEED_SMOOTHING);
            boids[i].vy *= (1 + SPEED_SMOOTHING);
        } else if (speed > targetSpeed) {
            boids[i].vx *= (1 - SPEED_SMOOTHING);
            boids[i].vy *= (1 - SPEED_SMOOTHING);
        }

        boids[i].x += boids[i].vx;
        boids[i].y += boids[i].vy;

        if (boids[i].x < 0) boids[i].x += SCREEN_WIDTH;
        if (boids[i].x >= SCREEN_WIDTH) boids[i].x -= SCREEN_WIDTH;
        if (boids[i].y < 0) boids[i].y += SCREEN_HEIGHT;
        if (boids[i].y >= SCREEN_HEIGHT) boids[i].y -= SCREEN_HEIGHT;
    }
}

void render_boids(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    for (int i = 0; i < NUM_BOIDS; i++) {
        float angle = atan2(boids[i].vy, boids[i].vx);

        SDL_Point p1 = { (int)(boids[i].x + cos(angle) * 8), (int)(boids[i].y + sin(angle) * 8) };
        SDL_Point p2 = { (int)(boids[i].x + cos(angle + 2.5) * 5), (int)(boids[i].y + sin(angle + 2.5) * 5) };
        SDL_Point p3 = { (int)(boids[i].x + cos(angle - 2.5) * 5), (int)(boids[i].y + sin(angle - 2.5) * 5) };

        SDL_Point triangle[4] = { p1, p2, p3, p1 };
        SDL_RenderDrawLines(renderer, triangle, 4);
    }
}
