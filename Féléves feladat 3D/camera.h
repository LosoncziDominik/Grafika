#ifndef CAMERA_H
#define CAMERA_H

#include "include/glad/glad.h"
#include "include/KHR/khrplatform.h"
#include <SDL2/SDL.h>

typedef struct {
    float x, y, z;
    float pitch, yaw;
    float speed;
    int lastMouseX, lastMouseY;
    int firstMouse;
} Camera;

void init_camera(Camera* cam);
void update_camera(Camera* cam, const Uint8* keystates, int mouseX, int mouseY);
void set_camera_view(Camera* cam);

#endif
