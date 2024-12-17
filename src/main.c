#include <stdio.h> 
#include <math.h>
#include <SDL2/SDL.h>

#include "Model.h"  // Model.h includes Vector.h

/* -Camera.h
 * Contains all camera relevant information
 * ratio, fov, near, far, bottom, top, left, right
 * projection_matrix
 *
 * - Already includes Matrix.h 
 *   Matrix.h also includes Vector.h
 */
#include "Camera.h" // Camera.h includes Matrix.h

int main()
{
    int width = 1920;
    int height = 1080;

    Matrix4x4 transform_matrix= {0};

    InitializeCamera(width, height, 90.0, 0.1, 100);
    printf("FOV: %f\n", fov);
    SetFrustumMatrix(&projection_matrix, near, far, left, right, bottom, top);

    SDL_Window *window;
    SDL_Renderer * renderer;
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Simple 3D demo",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, 
            SDL_RENDERER_ACCELERATED |
            SDL_RENDERER_TARGETTEXTURE);
    if(window == NULL)
    {
        printf("Couldn't create window: %s\n", SDL_GetError());
        return 0;
    }


    // After init, load model
    Model cube = {0};
    Model monkey = {0};

    //load_model(&cube, "models/cube.obj");
    //load_model(&monkey, "models/monkey.obj");
    load_model(&monkey, "models/stair.obj");

    cube.position.x = 0;
    cube.position.y = 0;
    cube.position.z = -30;

    monkey.position.z = -15;
    monkey.position.x = 0;
    //monkey.rotation.y = 45;

    int quit = 0;
    SDL_Event event;

    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit = 1;
            } 
        }
        SDL_RenderClear(renderer);

        //cube.rotation.x += 0.015;
        //cube.rotation.y += 0.01;
        //cube.rotation.z += 0.008;

        monkey.rotation.y -= 0.02;
        monkey.rotation.x += 0.01;
        // Draw to screen

        //calculate_vertices(&cube, &transform_matrix, &projection_matrix);
        //sort_faces(&cube);
        //draw_model(&cube, renderer);

        calculate_vertices(&monkey, &transform_matrix, &projection_matrix);
        sort_faces(&monkey);
        draw_model(&monkey, renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}
