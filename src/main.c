#include <stdio.h> 
#include <math.h>
#include <SDL2/SDL.h>

#include "Matrix.h" // Currenty Matrix.h includes Vector.h

void set_projection_matrix(float fov, float far, float near, Matrix4x4 *matrix);
void set_identity_matrix(Matrix4x4 *matrix);
void set_x_rotation_matrix(Matrix3x3 *matrix, float angle);
void set_y_rotation_matrix(Matrix3x3 *matrix, float angle);
void set_z_rotation_matrix(Matrix3x3 *matrix, float angle);


int main()
{
    // A set of point matricies describing a cube
    Vector3f CubeVerticies[8] = {
        {-1, -1, -1}, // A
        { 1, -1, -1}, // B
        { 1, -1,  1}, // C
        {-1, -1,  1}, // D
        {-1, 1, -1}, // A1
        { 1, 1, -1}, // B1
        { 1, 1,  1}, // C1
        {-1, 1,  1}  // D1
    };

    float x_angle = 0;
    float y_angle = 0;
    float z_angle = 0;

    Matrix4x4 camera_matrix = {0};
    set_identity_matrix(&camera_matrix);
    camera_matrix.m31 = 0; // Y coordinate
    camera_matrix.m32 = 3; // Z coordinate
    float fov = 90;
    float near = 0.1;
    float far = 100;

    Matrix4x4 projection_matrix = {0};
    set_projection_matrix(fov, near, far, &projection_matrix);

    SDL_Window *window;
    SDL_Renderer * renderer;
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Simple 3D demo",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            600,
                            600,
                            SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(window == NULL)
    {
        printf("Couldn't create window: %s\n", SDL_GetError());
        return 0;
    }
    
    int quit = 0;
    SDL_Event event;
    while(!quit){
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit = 1;
            } 
        }
        SDL_RenderClear(renderer);

        //camera_matrix.m32 -= 0.1;
        x_angle += 0.01;
        y_angle += 0.01;
        z_angle += 0.01;
        // Draw to screen


        // Save projected points
        Vector2f screen_points[8] = {0};
        
        // Draw the cube using a projection matrix
        for(int i = 0; i < 8; i++)
        {
            Vector3f result_vertex = {0};
            Vector3f camera_result_vertex = {0};
            Vector3f *vertex = &CubeVerticies[i];

            // Apply x-rotation matrix
            Matrix3x3 x_rotation_matrix = {0};
            Vector3f x_rotation_result = {0};
            set_x_rotation_matrix(&x_rotation_matrix, x_angle);
            MultiplyPointBy3x3(vertex, &x_rotation_matrix, &x_rotation_result);
            
            // Apply y-rotation matrix
            Matrix3x3 y_rotation_matrix = {0};
            Vector3f y_rotation_result = {0};
            set_y_rotation_matrix(&y_rotation_matrix, y_angle);
            MultiplyPointBy3x3(&x_rotation_result, &y_rotation_matrix, &y_rotation_result);

            // Apply z-rotation matrix
            Matrix3x3 z_rotation_matrix = {0};
            Vector3f z_rotation_result = {0};
            set_z_rotation_matrix(&z_rotation_matrix, z_angle);
            MultiplyPointBy3x3(&y_rotation_result, &z_rotation_matrix, &z_rotation_result);


            // Apply camera matrix 
            MultiplyPointBy4x4(&z_rotation_result, &camera_matrix, &camera_result_vertex);
            
            // Apply projection
            MultiplyPointBy4x4(&camera_result_vertex, &projection_matrix, &result_vertex);
            // Let's see if the vertex point is visible
            if(result_vertex.x > 1 || result_vertex.x < -1 ||
                result_vertex.y > 1 || result_vertex.y < -1)
                continue;   // Point isn't visible

            // Convert to screen space
            int x = (result_vertex.x + 1) * 0.5 * 600;
            int y = (result_vertex.y + 1) * 0.5 * 600;
            //printf("Screen coords (%d, %d)\n", x, y);

            screen_points[i].x = x;
            screen_points[i].y = y;

            // Draw a point

            SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
            SDL_RenderDrawPoint(renderer, x, y);
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        }


        // Draw lines between the screen points of the cube
        for(int i = 0; i < 8; i++)
        {
            SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
            if(i <= 3)
            {
                if(i == 3)
                {
                    SDL_RenderDrawLine(renderer, screen_points[i].x, screen_points[i].y, screen_points[0].x, screen_points[0].y);
                }else
                {
                    SDL_RenderDrawLine(renderer, screen_points[i].x, screen_points[i].y, screen_points[i+1].x, screen_points[i+1].y);
                }
                SDL_RenderDrawLine(renderer, screen_points[i].x, screen_points[i].y, screen_points[i+4].x, screen_points[i+4].y);
            }else
            {
                if(i == 7)
                {
                    SDL_RenderDrawLine(renderer, screen_points[i].x, screen_points[i].y, screen_points[i - 3].x, screen_points[i-3].y);
                }else
                {
                    SDL_RenderDrawLine(renderer, screen_points[i].x, screen_points[i].y, screen_points[i+1].x, screen_points[i+1].y);
                }
            }
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}


// Sets up a simple projection matrix, without left, right, top, down values
void set_projection_matrix(float fov, float far, float near, Matrix4x4 *matrix)
{
    float scale = 1 / tan(fov/2 * M_PI/180);
    matrix->m00 = scale;
    matrix->m11 = scale;
    matrix->m22 = -far / (far - near);
    matrix->m23 = -1;
    matrix->m32 = (-far * near) / (far - near);
}

void set_identity_matrix(Matrix4x4 *matrix)
{
    matrix->m00 = 1;
    matrix->m11 = 1;
    matrix->m22 = 1;
    matrix->m33 = 1;
}

void set_x_rotation_matrix(Matrix3x3 *matrix, float angle)
{
    matrix->m00 = 1;
    matrix->m11 = cos(angle);
    matrix->m12 = -sin(angle);
    matrix->m21 = sin(angle);
    matrix->m22 = cos(angle);
}

void set_y_rotation_matrix(Matrix3x3 *matrix, float angle)
{
    matrix->m00 = cos(angle);
    matrix->m02 = sin(angle);
    matrix->m11 = 1;
    matrix->m20 = -sin(angle);
    matrix->m22 = cos(angle);
}

void set_z_rotation_matrix(Matrix3x3 *matrix, float angle)
{
    matrix->m00 = cos(angle);
    matrix->m01 = -sin(angle);
    matrix->m10 = sin(angle);
    matrix->m11 = cos(angle);
}
