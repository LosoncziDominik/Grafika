#include "boid.h"
#include <math.h>
#include <stdlib.h>

Boid boids[NUM_BOIDS];
Player player;
Blood blood_drops[MAX_BLOOD];
int bloodCount = 0;


void init_boids() {
    for (int i = 0; i < NUM_BOIDS; i++) {
        boids[i].x = rand() % SCREEN_WIDTH;
        boids[i].y = rand() % SCREEN_HEIGHT;
        boids[i].vx = ((rand() % 200) / 100.0f - 1) * MAX_SPEED;
        boids[i].vy = ((rand() % 200) / 100.0f - 1) * MAX_SPEED;
        boids[i].alive = true;
    }
}

void init_player(){
    player.x = SCREEN_WIDTH / 2;
    player.y = SCREEN_HEIGHT * 3 / 4;
    player.vx = 0;
    player.vy = 0;
}

void update_boids(SDL_Renderer* renderer) {
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
                
                float pdx = player.x - boids[i].x;
                float pdy = player.y - boids[i].y;
                float pdist = sqrt(pdx * pdx + pdy * pdy);

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

                if (pdist < VIEW_RADIUS){
                    float runx = -pdx / pdist;
                    float runy = -pdy / pdist;

                    boids[i].vx = runx * 1.5;
                    boids[i].vy = runy * 1.5;
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

        warp(&boids[i].x, &boids[i].y);

        if (fabs(boids[i].x - player.x ) < 10 && fabs(boids[i].y - player.y ) < 10){
            boids[i].alive = false;

            if(bloodCount < MAX_BLOOD){
                blood_drops[bloodCount].x = boids[i].x + rand() % 10 - 3;;
                blood_drops[bloodCount].y = boids[i].y + rand() % 10 - 3;;
                blood_drops[bloodCount].active = true;
                bloodCount++;
            }
        }

       

    }
}

void update_player() {
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    float vx = 0, vy = 0;

    if (keystates[SDL_SCANCODE_W]) vy -= 1;
    if (keystates[SDL_SCANCODE_S]) vy += 1;
    if (keystates[SDL_SCANCODE_A]) vx -= 1;
    if (keystates[SDL_SCANCODE_D]) vx += 1;

    float norm = sqrt(vx * vx + vy * vy);
    if (norm > 0) {
        vx = (vx / norm) * PLAYER_SPEED;
        vy = (vy / norm) * PLAYER_SPEED;

        player.vx += (vx - player.vx) * TURNING_RATE;
        player.vy += (vy - player.vy) * TURNING_RATE;

        player.x += vx;
        player.y += vy;

        player.angle = atan2(player.vy, player.vx);

    }

    warp(&player.x, &player.y);

    player.counter = score_counter();
}


void render_boids(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    for (int i = 0; i < NUM_BOIDS; i++) {
        if(boids[i].alive == true){

            float angle = atan2(boids[i].vy, boids[i].vx);

            SDL_Point p1 = { (int)(boids[i].x + cos(angle) * 8), (int)(boids[i].y + sin(angle) * 8) };
            SDL_Point p2 = { (int)(boids[i].x + cos(angle + 2.5) * 5), (int)(boids[i].y + sin(angle + 2.5) * 5) };
            SDL_Point p3 = { (int)(boids[i].x + cos(angle - 2.5) * 5), (int)(boids[i].y + sin(angle - 2.5) * 5) };
    
            SDL_Point triangle[4] = { p1, p2, p3, p1 };
            SDL_RenderDrawLines(renderer, triangle, 4);
        }
        
    }
}

void render_player(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    player.angle = atan2(player.vy, player.vx);

    SDL_Point p1 = { (int)(player.x + cos(player.angle) * 8), (int)(player.y + sin(player.angle) * 8) };
    SDL_Point p2 = { (int)(player.x + cos(player.angle + 2.5) * 5), (int)(player.y + sin(player.angle + 2.5) * 5) };
    SDL_Point p3 = { (int)(player.x + cos(player.angle - 2.5) * 5), (int)(player.y + sin(player.angle - 2.5) * 5) };

    SDL_Point triangle[4] = { p1, p2, p3, p1 };
    SDL_RenderDrawLines(renderer, triangle, 4);
}

void warp (float* x, float* y){
    if (*x < 0) *x += SCREEN_WIDTH;
    if (*x >= SCREEN_WIDTH) *x -= SCREEN_WIDTH;
    if (*y < 0) *y += SCREEN_HEIGHT;
    if (*y >= SCREEN_HEIGHT) *y -= SCREEN_HEIGHT;
}

void draw_score(SDL_Renderer* renderer, const char* score, int x, int y, TTF_Font* font, SDL_Color color){
    SDL_Surface* surface = TTF_RenderText_Blended(font, score, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dstrect = { x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

int score_counter(){
    int counter = 0;

    for(int i = 0; i < NUM_BOIDS; i++) if(!boids[i].alive) counter++;

    return counter;
}

void render_blood(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 180);

    for (int i = 0; i < bloodCount; i++)
    {
        if(blood_drops[i].active){
            for (int j = 0; j < 10; j++)
            {
                SDL_RenderDrawPoint(renderer, (int)(blood_drops[i].x), (int)(blood_drops[i].y));
            }
        }
    }
}

    int get_player_score(){
    return player.counter;
}