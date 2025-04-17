#include <math.h>
#include <stdlib.h>
#include "boids.h"
#include <stdio.h>


Boid boids[NUM_BOIDS];
Player player;
Vertex vertices[8][1000];
Face faces[8][1000];
int vertex_counts[8] = {0};
int face_counts[8] = {0};
Torus torus[3];
Vec3 boid_pos[NUM_BOIDS];
Vec3 boid_vel;

void init_boids() {
    for (int i = 0; i < NUM_BOIDS; i++) {
        boids[i].x = (rand() % SPACE_WIDTH) - SPACE_WIDTH / 2;
        boids[i].y = (rand() % SPACE_HEIGHT) - SPACE_HEIGHT / 2;
        boids[i].z = (rand() % SPACE_DEPTH) - SPACE_DEPTH / 2;

        boids[i].vx = ((float)rand() / RAND_MAX - 0.5f) * 0.3f;
        boids[i].vy = ((float)rand() / RAND_MAX - 0.5f) * 0.3f;
        boids[i].vz = ((float)rand() / RAND_MAX - 0.5f) * 0.3f;
        boids[i].alive = true;
        boids[i].targetSpeed = MAX_SPEED * (0.7f + (rand() % 5) * 0.01f);
        boids[i].r = (rand() % 9) / 10.0f;
        boids[i].g = (rand() % 9) / 10.0f;
        boids[i].b = (rand() % 9) / 10.0f;
        boids[i].frame_index = rand() % 8;
        boids[i].frame_timer = ((float)rand() / RAND_MAX) * 0.5f;

        boid_pos[i].x = boids[i].x;
        boid_pos[i].y = boids[i].y;
        boid_pos[i].z = boids[i].z;

    }

    load_obj("hal.obj", 0);
    load_obj("hal3.obj", 1);
    load_obj("hal4.obj", 2);
    load_obj("hal3.obj", 3);
    load_obj("hal.obj", 4);
    load_obj("hal1.obj", 5);
    load_obj("hal2.obj", 6);
    load_obj("hal1.obj", 7);
}

void init_player(){
    player.x = SPACE_WIDTH / 2;
    player.y = SPACE_HEIGHT * 3 / 4;
    player.z = SPACE_DEPTH * 4 / 5;
    player.vx = 0;
    player.vy = 0;
    player.vz = 0;
}

void init_torus(){
    torus[0].x = 80;
    torus[0].y = 30;
    torus[0].z = -40;

    torus[1].x = -80;
    torus[1].y = 30;
    torus[1].z = -40;

    torus[2].x = 0;
    torus[2].y = -50;
    torus[2].z = 40;
    
    for(int i = 0 ; i < 3; i++){
        torus[i].R = 30.0f;
        torus[i].r = 10.0f; 
    }
}

void update_boids() {

    for (int i = 0; i < NUM_BOIDS; i++) {
        float avgX = 0, avgY = 0, avgZ = 0;
        float avgVX = 0, avgVY = 0, avgVZ = 0;
        float sepX = 0, sepY = 0, sepZ = 0;
        int count = 0;

        for (int j = 0; j < NUM_BOIDS; j++) {
            if (i == j) continue;

            float dx = boids[j].x - boids[i].x;
            float dy = boids[j].y - boids[i].y;
            float dz = boids[j].z - boids[i].z;
            float dist = sqrt(dx * dx + dy * dy + dz * dz);

            if (dist < VIEW_RADIUS) {
                avgX += boids[j].x;
                avgY += boids[j].y;
                avgZ += boids[j].z;
                avgVX += boids[j].vx;
                avgVY += boids[j].vy;
                avgVZ += boids[j].vz;
                count++;
            }

            if (dist < SEPARATION_RADIUS) {
                sepX -= (dx * SPEED_SMOOTHING);
                sepY -= (dy * SPEED_SMOOTHING);
                sepZ -= (dz * SPEED_SMOOTHING);
            }
        }

        float alignWeight = ALIGNMENT_WEIGHT;
        float cohesionWeight = COHESION_WEIGHT;
        if (count > 0) {
            alignWeight = ALIGNMENT_WEIGHT / (1.0f + 0.05f * count);
            cohesionWeight = COHESION_WEIGHT / (1.0f + 0.05f * count);

            avgX /= count;
            avgY /= count;
            avgZ /= count;
            avgVX /= count;
            avgVY /= count;
            avgVZ /= count;

            float targetVX = (avgX - boids[i].x) * cohesionWeight;
            float targetVY = (avgY - boids[i].y) * cohesionWeight;
            float targetVZ = (avgZ - boids[i].z) * cohesionWeight;

            boids[i].vx += (targetVX - boids[i].vx) * TURNING_RATE ;
            boids[i].vy += (targetVY - boids[i].vy) * TURNING_RATE ;
            boids[i].vz += (targetVZ - boids[i].vz) * TURNING_RATE ;

            boids[i].vx += (avgVX - boids[i].vx) * alignWeight;
            boids[i].vy += (avgVY - boids[i].vy) * alignWeight;
            boids[i].vz += (avgVZ - boids[i].vz) * alignWeight;
        }

        boids[i].vx += sepX * SEPARATION_WEIGHT;
        boids[i].vy += sepY * SEPARATION_WEIGHT;
        boids[i].vz += sepZ * SEPARATION_WEIGHT;

        const float margin = 10.0f;
        const float avoid = 0.01f;
        if (boids[i].x >  SPACE_WIDTH /2 - margin) boids[i].vx -= avoid;
        if (boids[i].x < -SPACE_WIDTH /2 + margin) boids[i].vx += avoid;
        if (boids[i].y >  SPACE_HEIGHT/2 - margin) boids[i].vy -= avoid;
        if (boids[i].y < -SPACE_HEIGHT/2 + margin) boids[i].vy += avoid;
        if (boids[i].z >  SPACE_DEPTH /2 - margin) boids[i].vz -= avoid;
        if (boids[i].z < -SPACE_DEPTH /2 + margin) boids[i].vz += avoid;

        float speed = sqrt(boids[i].vx * boids[i].vx + boids[i].vy * boids[i].vy + boids[i].vz * boids[i].vz);
        boids[i].speeds = speed;

        float targetSpeed = MAX_SPEED * 0.7f + (rand() % 5) * 0.1f;
        boids[i].targetspeeds = targetSpeed;

        if (speed > MAX_SPEED) {
            float scale = MAX_SPEED / speed;
            boids[i].vx *= scale;
            boids[i].vy *= scale;
            boids[i].vz *= scale;
        }

        if (speed < targetSpeed) {
            boids[i].vx *= (1 + SPEED_SMOOTHING);
            boids[i].vy *= (1 + SPEED_SMOOTHING);
            boids[i].vz *= (1 + SPEED_SMOOTHING);
        } else if (speed > targetSpeed){
            boids[i].vx *= (1 - SPEED_SMOOTHING);
            boids[i].vy *= (1 - SPEED_SMOOTHING);
            boids[i].vz *= (1 - SPEED_SMOOTHING);
        }
        boid_pos[i].x = boids[i].x;
        boid_pos[i].y = boids[i].y;
        boid_pos[i].z = boids[i].z;

        boids[i].x += boids[i].vx;
        boids[i].y += boids[i].vy;
        boids[i].z += boids[i].vz;

        boids[i].frame_timer -= 0.064f;
        if (boids[i].frame_timer <= 0) {
            boids[i].frame_index = (boids[i].frame_index + 1) % 8;
            boids[i].frame_timer = 0.2f + ((float)rand() / RAND_MAX) * 0.3f;
        }
        /*for (int k = 0; k < 3; k++)
        {
            Vec3 steer = avoid_torus(boid_pos[i], get_boid_vel(i), get_center(k), torus[k].R, torus[k].r);
            boids[i].vx += steer.x;
            boids[i].vy += steer.y;
            boids[i].vz += steer.z;
        }*/
        
        
    }
}

void wrap_boid(Boid* b) {
    if (b->x < -SPACE_WIDTH / 2)  b->x += SPACE_WIDTH;
    if (b->x >  SPACE_WIDTH / 2)  b->x -= SPACE_WIDTH;

    if (b->y < -SPACE_HEIGHT / 2) b->y += SPACE_HEIGHT;
    if (b->y >  SPACE_HEIGHT / 2) b->y -= SPACE_HEIGHT;

    if (b->z < -SPACE_DEPTH / 2)  b->z += SPACE_DEPTH;
    if (b->z >  SPACE_DEPTH / 2)  b->z -= SPACE_DEPTH;
}


int score_counter(){
    int counter = 0;

    for(int i = 0; i < NUM_BOIDS; i++) if(!boids[i].alive) counter++;

    return counter;
}

int get_player_score(){
    return player.counter;
}

void render_boids() {
    for (int i = 0; i < NUM_BOIDS; i++) {
        if (!boids[i].alive) continue;

        float angleY = atan2(boids[i].vx, boids[i].vz) * 180.0f / M_PI;

        render_model_instance(boids[i].x, boids[i].y, boids[i].z, angleY, i);

        /*for (int k = 0; k < 3; k++) {
            Vec3 pos = boid_pos[i];
            Vec3 vel = get_boid_vel(i);
            Vec3 steer = avoid_torus(pos, vel, get_center(k), torus[k].R, torus[k].r);
    
            if (vec3_length(steer) > 0.01f) {
                glColor3f(1, 0, 0); // piros vonal
                glBegin(GL_LINES);
                glVertex3f(pos.x, pos.y, pos.z);
                glVertex3f(pos.x - steer.x, pos.y - steer.y, pos.z - steer.z);
                glEnd();
            }
        }
    
        for (int i = 0; i < 3; i++) {
            draw_torus_debug_wire(i);
        }*/
    }
}

void draw_plane(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glBegin(GL_QUADS);
        glColor3f(1, 1, 0);
        glVertex3f(-0.5f, 0, -0.5f);
        glVertex3f( 0.5f, 0, -0.5f);
        glVertex3f( 0.5f, 0,  0.5f);
        glVertex3f(-0.5f, 0,  0.5f);
    glEnd();
    glPopMatrix();
}


void draw_bounds() {
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            for (int k = -1; k <= 1; k += 2) {
                glVertex3f(i * SPACE_WIDTH/2, j * SPACE_HEIGHT/2, -SPACE_DEPTH/2);
                glVertex3f(i * SPACE_WIDTH/2, j * SPACE_HEIGHT/2,  SPACE_DEPTH/2);
            }
        }
    }
    glEnd();
}

void whatspeed(){
    printf("\n%f\n%f\n", boids[0].speeds, boids[0].targetspeeds);
    printf("\nx:%f\ny:%f\nz:%f\n", boids[0].x, boids[0].y, boids[0].z);
}

void load_obj(const char* filename, int index) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Could not open OBJ file\n");
        exit(1);
    }

    vertex_counts[index] = 0;
    face_counts[index] = 0;

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            float x, y, z;
            if (sscanf(line, "v %f %f %f", &x, &y, &z) == 3) {
                vertices[index][vertex_counts[index]++] = (Vertex){x, y, z};
            }
        } else if (strncmp(line, "f ", 2) == 0) {
            int v[4];
            int matches = sscanf(line, "f %d %d %d %d", &v[0], &v[1], &v[2], &v[3]);

            if (matches == 3) {
                faces[index][face_counts[index]++] = (Face){v[0], v[1], v[2]};
            } else if (matches == 4) {
                faces[index][face_counts[index]++] = (Face){v[0], v[1], v[2]};
                faces[index][face_counts[index]++] = (Face){v[0], v[2], v[3]};
            } else {
                int vtx[4], vt[4], vn[4];
                int count = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                    &vtx[0], &vt[0], &vn[0], &vtx[1], &vt[1], &vn[1], &vtx[2], &vt[2], &vn[2], &vtx[3], &vt[3], &vn[3]);

                if (count >= 9) {
                    faces[index][face_counts[index]++] = (Face){vtx[0], vtx[1], vtx[2]};
                    if (count == 12) {
                        faces[index][face_counts[index]++] = (Face){vtx[0], vtx[2], vtx[3]};
                    }
                } else {
                    count = sscanf(line, "f %d//%d %d//%d %d//%d %d//%d",
                        &vtx[0], &vn[0], &vtx[1], &vn[1], &vtx[2], &vn[2], &vtx[3], &vn[3]);

                    if (count >= 6) {
                        faces[index][face_counts[index]++] = (Face){vtx[0], vtx[1], vtx[2]};
                        if (count == 8) {
                            faces[index][face_counts[index]++] = (Face){vtx[0], vtx[2], vtx[3]};
                        }
                    }
                }
            }
        }
    }

    fclose(file);
}

void render_model_instance(float x, float y, float z, float angle, int index) {
    int frame = boids[index].frame_index + boids[index].frame_timer;

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, 0, 1, 0);
    glColor3f(boids[index].r, boids[index].g, boids[index].b);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < face_counts[frame]; i++) {
        int vi1 = faces[frame][i].v1 - 1;
        int vi2 = faces[frame][i].v2 - 1;
        int vi3 = faces[frame][i].v3 - 1;

        glVertex3f(vertices[frame][vi1].x, vertices[frame][vi1].y, vertices[frame][vi1].z);
        glVertex3f(vertices[frame][vi2].x, vertices[frame][vi2].y, vertices[frame][vi2].z);
        glVertex3f(vertices[frame][vi3].x, vertices[frame][vi3].y, vertices[frame][vi3].z);
    }
    glEnd();

    glPopMatrix();
}

void create_donut_shape(int index) {
    int numSides = 32;
    int numRings = 32;
    float R = torus[index].R;
    float r = torus[index].r;

    for (int j = 0; j < numRings; ++j) {
        float theta = 2.0f * M_PI * j / numRings;
        float nextTheta = 2.0f * M_PI * (j + 1) / numRings;

        for (int k = 0; k < numSides; ++k) {
            float phi = 2.0f * M_PI * k / numSides;
            float nextPhi = 2.0f * M_PI * (k + 1) / numSides;

            // Négy szomszédos pont a torusz felületén
            float x0 = (R + r * cos(phi)) * cos(theta);
            float y0 = (R + r * cos(phi)) * sin(theta);
            float z0 = r * sin(phi);

            float x1 = (R + r * cos(nextPhi)) * cos(theta);
            float y1 = (R + r * cos(nextPhi)) * sin(theta);
            float z1 = r * sin(nextPhi);

            float x2 = (R + r * cos(nextPhi)) * cos(nextTheta);
            float y2 = (R + r * cos(nextPhi)) * sin(nextTheta);
            float z2 = r * sin(nextPhi);

            float x3 = (R + r * cos(phi)) * cos(nextTheta);
            float y3 = (R + r * cos(phi)) * sin(nextTheta);
            float z3 = r * sin(phi);

            glBegin(GL_TRIANGLES);
            glVertex3f(x0, y0, z0);
            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);

            glVertex3f(x0, y0, z0);
            glVertex3f(x2, y2, z2);
            glVertex3f(x3, y3, z3);
            glEnd();
        }
    }   
}

int get_torus_x(int index){
    return torus[index].x;
}

int get_torus_y(int index){
    return torus[index].y;
}

int get_torus_z(int index){
    return torus[index].z;
}

Vec3 get_center(int index)
{
    Vec3 torus_center;
    torus_center.x = torus[index].x;
    torus_center.y = torus[index].y;
    torus_center.z = torus[index].z;

    return torus_center;
}

Vec3 get_boid_vel(int index){
    Vec3 boid_vel = (Vec3){boids[index].vx, boids[index].vy, boids[index].vz,};

    return boid_vel;
}

Vec3 vec3_sub(Vec3 a, Vec3 b) {
    return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3 vec3_add(Vec3 a, Vec3 b) {
    return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3 vec3_mul(Vec3 v, float s) {
    return (Vec3){v.x * s, v.y * s, v.z * s};
}

float vec3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vec3_length(Vec3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec3 vec3_normalize(Vec3 v) {
    float len = vec3_length(v);
    if (len > 0.0001f)
        return vec3_mul(v, 1.0f / len);
    return (Vec3){0, 0, 0};
}

Vec3 avoid_torus(Vec3 boidPos, Vec3 boidVel, Vec3 torusCenter, float R, float r) {
    Vec3 rel = vec3_sub(boidPos, torusCenter);

    // síkbeli távolság a torusz középponttól (XZ sík)
    float flatX = rel.x;
    float flatZ = rel.z;
    float flatLen = sqrtf(flatX * flatX + flatZ * flatZ);

    // három feltétel: a boid a torusz testében van?
    bool inside_outer_wall = flatLen < (R + r);
    bool inside_inner_wall = flatLen > (R - r);
    bool inside_vertical   = fabsf(rel.y) < r;

    if (inside_inner_wall && inside_outer_wall && inside_vertical) {
        // A boid benne van a torusz testében → taszító erő

        // irány a torusz körkörös falából kifelé
        float len = sqrtf(flatX * flatX + flatZ * flatZ);
        Vec3 ringNormal = (len > 0.0001f)
            ? (Vec3){flatX / len, 0, flatZ / len}
            : (Vec3){1, 0, 0};

        // legközelebbi pont a torusz "forgáskörén"
        Vec3 ringCenter = vec3_add(torusCenter, vec3_mul(ringNormal, R));

        // taszítás iránya: boid → körív
        Vec3 fromRing = vec3_sub(boidPos, ringCenter);

        // opcionálisan: rajzolj piros vonalat a taszítás irányába
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex3f(boidPos.x, boidPos.y, boidPos.z);
        glVertex3f(boidPos.x + fromRing.x, boidPos.y + fromRing.y, boidPos.z + fromRing.z);
        glEnd();

        return vec3_mul(vec3_normalize(fromRing), 5.5f);
    }

    // ha a boid szemből repül a lyukba, ne taszítsuk
    Vec3 toCenter = vec3_normalize(vec3_sub(torusCenter, boidPos));
    float alignment = vec3_dot(vec3_normalize(boidVel), toCenter);

    if (alignment > 0.9f && flatLen > (R - r) && flatLen < (R + r)) {
        return (Vec3){0, 0, 0};
    }

    // nem kell semmit csinálni
    return (Vec3){0, 0, 0};
}

void draw_torus_debug_wire(int index) {
    int numSides = 32;
    int numRings = 32;

    float R = torus[index].R;
    float r = torus[index].r;
    float cx = torus[index].x;
    float cy = torus[index].y;
    float cz = torus[index].z;

    glColor3f(0.2f, 1.0f, 0.2f); // világoszöld szín a drótkerethez
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // csak vonalak

    for (int j = 0; j < numRings; ++j) {
        float theta = 2.0f * M_PI * j / numRings;
        float nextTheta = 2.0f * M_PI * (j + 1) / numRings;

        for (int k = 0; k < numSides; ++k) {
            float phi = 2.0f * M_PI * k / numSides;
            float nextPhi = 2.0f * M_PI * (k + 1) / numSides;

            float x0 = (R + r * cosf(phi)) * cosf(theta) + cx;
            float y0 = (R + r * cosf(phi)) * sinf(theta) + cy;
            float z0 = r * sinf(phi) + cz;

            float x1 = (R + r * cosf(nextPhi)) * cosf(theta) + cx;
            float y1 = (R + r * cosf(nextPhi)) * sinf(theta) + cy;
            float z1 = r * sinf(nextPhi) + cz;

            float x2 = (R + r * cosf(nextPhi)) * cosf(nextTheta) + cx;
            float y2 = (R + r * cosf(nextPhi)) * sinf(nextTheta) + cy;
            float z2 = r * sinf(nextPhi) + cz;

            float x3 = (R + r * cosf(phi)) * cosf(nextTheta) + cx;
            float y3 = (R + r * cosf(phi)) * sinf(nextTheta) + cy;
            float z3 = r * sinf(phi) + cz;

            glBegin(GL_LINE_LOOP);
            glVertex3f(x0, y0, z0);
            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
            glVertex3f(x3, y3, z3);
            glEnd();
        }
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // visszaállítjuk a normál rajzolást
}



