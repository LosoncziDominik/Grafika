#include "boids.h"
#include "camera.h"

#ifndef CALLBACK
#define CALLBACK
#endif
#include <GL/glu.h>

Camera camera;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    SDL_Window* window = SDL_CreateWindow("Boid Szimulacio", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0 / 600.0, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);


    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.7f, 0.4f, 1.0f);

    SDL_SetRelativeMouseMode(SDL_TRUE);


    init_camera(&camera);
    init_boids();
    init_torus();

    bool running = true;
    SDL_Event event;

    while (running) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || 
            (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            running = false;
        }

        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_TAB) {
                whatspeed();
            }
        }
    }

    int mouseX, mouseY;
    SDL_GetRelativeMouseState(&mouseX, &mouseY);
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    update_camera(&camera, keystates, mouseX, mouseY);
    update_boids();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    set_camera_view(&camera);

    glPushMatrix();
    glTranslatef(0, 0, 0);
    glBegin(GL_TRIANGLES);
        glColor3f(1, 1, 0);
        glVertex3f(-1, -1, 0);
        glVertex3f( 1, -1, 0);
        glVertex3f( 0,  1, 0);
    glEnd();
    glPopMatrix();


    
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        //glColor3f(1, 0, 1);
        glTranslatef(get_torus_x(i), get_torus_y(i), get_torus_z(i));
        create_donut_shape(i);
        glPopMatrix();
    }

    
    render_boids();
    draw_plane(10, 0, 0);
    draw_bounds();

    SDL_GL_SwapWindow(window);
    SDL_Delay(16);
}

SDL_GL_DeleteContext(context);
SDL_DestroyWindow(window);
SDL_Quit();
return 0;
}