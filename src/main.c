#include <stdio.h> 
#include <math.h>
#include <SDL2/SDL.h>

#include "Matrix.h" // Matrix.h includes Vector.h
#include "Model.h"  // Model.h includes Vector.h

void set_projection_matrix(float fov, float far, float near, Matrix4x4 *matrix);
void set_frustum_matrix(Matrix4x4 *matrix, float near, float far, float left, float right, float bottom, float top);
void set_identity_matrix(Matrix4x4 *matrix);
void set_x_rotation_matrix(Matrix4x4 *matrix, float angle);
void set_y_rotation_matrix(Matrix4x4 *matrix, float angle);
void set_z_rotation_matrix(Matrix4x4 *matrix, float angle);

void apply_position(Vector3f position, Matrix4x4 *camera_matrix);
void project_at_position(Vector3f position, Vector3f *vector, Matrix4x4 *matrix, Vector3f *out);


int main()
{
    float x_angle = 0;
    float y_angle = 0;
    float z_angle = 0;

    Vector3f position = {0};
    position.x = 0;
    position.y = 0;
    position.z = 30;

    float fov = 90;
    float near = 0.1;
    float far = 100;

    int width = 1920;
    int height = 1080;

    float ratio = (float)width/(float)height;
    printf("ratio: %f\n", ratio);
    float top = tan(fov/2) * near;
    float bottom = -top;
    float right = top * ratio;
    float left = bottom * ratio;

    Matrix4x4 camera_matrix = {0};
    set_identity_matrix(&camera_matrix);
    camera_matrix.m31 = 5; // Y coordinate
    camera_matrix.m32 = -30; // Z coordinate


    Matrix4x4 projection_matrix = {0};
    //set_projection_matrix(fov, near, far, &projection_matrix);
    set_frustum_matrix(&projection_matrix, near, far, left, right, bottom, top); 

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
    load_model(&cube, "models/Bf-109E3.obj");

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
        x_angle += 0.015;
        y_angle += 0.01;
        z_angle += 0.008;
        // Draw to screen
        //position.y -= 0.01;
        //position.z += 0.01; // Is a bit weird when combined with rotation and all

        // Calculate vertex points and save their screen coordinates
        for(int i = 0; i < cube.vertex_count; i++)
        {
            Vector3f result_vertex = {0};
            Vector3f position_result = {0};
            Vector3f camera_result_vertex = {0};
            Vector3f *vertex = &cube.vertices[i];

            // Apply x-rotation matrix
            Matrix4x4 x_rotation_matrix = {0};
            Vector3f x_rotation_result = {0};
            set_x_rotation_matrix(&x_rotation_matrix, x_angle);
            MultiplyPointBy4x4(vertex, &x_rotation_matrix, &x_rotation_result);

            // Apply y-rotation matrix
            Matrix4x4 y_rotation_matrix = {0};
            Vector3f y_rotation_result = {0};
            set_y_rotation_matrix(&y_rotation_matrix, y_angle);
            MultiplyPointBy4x4(&x_rotation_result, &y_rotation_matrix, &y_rotation_result);

            // Apply z-rotation matrix
            Matrix4x4 z_rotation_matrix = {0};
            Vector3f z_rotation_result = {0};
            set_z_rotation_matrix(&z_rotation_matrix, z_angle);
            MultiplyPointBy4x4(&y_rotation_result, &z_rotation_matrix, &z_rotation_result);


            // Update camera matrix with model position
            apply_position(position, &camera_matrix); 
            // Apply camera matrix 
            MultiplyPointBy4x4(&z_rotation_result, &camera_matrix, &camera_result_vertex);

            // Apply projection
            MultiplyPointBy4x4(&camera_result_vertex, &projection_matrix, &result_vertex);

            // Let's see if the vertex point is visible
            if(result_vertex.x > 1 || result_vertex.x < -1 ||
                    result_vertex.y > 1 || result_vertex.y < -1)
                continue;   // Point isn't visible

            // Convert to screen space
            int x = (result_vertex.x + 1) * 0.5 * width;
            int y = (result_vertex.y + 1) * 0.5 * height;
            float depth = result_vertex.z;

            Vector3f *v = &cube.screen[i];
            SDL_Vertex *vert = &cube.screen_vertices[i];
            vert->position.x = x;
            vert->position.y = y;
            v->z = depth * 100;
            
            vert->color.r = 255;
            vert->color.g = 255;
            vert->color.b = 255;
            vert->color.a = 255;
        }

        // Generate SDL vertices and draw the geometry

        Vector2f *ordered = malloc(cube.face_count * sizeof *ordered);
        memset(ordered, 0, cube.face_count * sizeof *ordered);
        int count = 0;
        // Copy faces depth and indexes to ordered
        for(int i = 0; i < cube.face_count; i++)
        {
            // This doesn't even take the average, just the depth of the first vertex.
            // Not sure if I need something more advanced though
            ordered[i].x = i;
            ordered[i].y = cube.screen[cube.faces[i][0].x].z;
            count++;
        }

        // Sort by biggest avg
        for(int i = 0; i < cube.face_count; i++)
        {
            for(int j = i + 1; j < cube.face_count; j++)
            {
                if(ordered[i].y < ordered[j].y)
                {
                    Vector2f temp = {0};
                    temp.x = ordered[i].x;
                    temp.y = ordered[i].y;

                    ordered[i].x = ordered[j].x;
                    ordered[i].y = ordered[j].y;

                    ordered[j].x = temp.x;
                    ordered[j].y = temp.y;
                }
            }
        }

        // Draw after sorting
        for(int i = count - 1; i >= 0; i--)
        {
            int index = (int)ordered[i].x;
            Vector3 *face;
            face = cube.faces[index];
            // Count number of vertices
            int v_n = 0;
            for(int j = 0; j < cube.face_data[index]; j++)
            {
                if(face[j].x == -1)
                    break;

                v_n++;
            }

            // Covert points to triangles
            // Implementing a simple triangle transformation
            int n = 1;
            SDL_Vertex g_vertices[3];

            for(int j = 1; j < cube.face_data[index]; j++)
            {
                if(j+1 >= v_n)
                    break;

                float scale = (float)(count-i)/count;

                g_vertices[0] = cube.screen_vertices[face[0].x];
                g_vertices[1] = cube.screen_vertices[face[j].x];
                g_vertices[2] = cube.screen_vertices[face[j+1].x];       


                // Apply depth gray scale
                for(int k = 0; k < 3; k++)
                {
                    g_vertices[k].color.r = 255 * scale;
                    g_vertices[k].color.g = 255 * scale;
                    g_vertices[k].color.b = 255 * scale;
                }


                Vector2f a = {0}, b = {0}, c = {0};
                a.x = g_vertices[0].position.x;
                a.y = g_vertices[0].position.y;
                b.x = g_vertices[1].position.x;
                b.y = g_vertices[1].position.y;
                c.x = g_vertices[2].position.x;
                c.y = g_vertices[2].position.y;



                Vector2f mod_a = {0};
                Sub(b, a, &mod_a);

                Vector2f mod_b = {0};
                Sub(c, a, &mod_b);

                if(Cross(mod_a, mod_b) >= 0)
                {
                    //g_vertices[0].color.r = cube.screen[cube.faces[i][j].x].z / 2048 * 255;
                    SDL_RenderGeometry(renderer, NULL, g_vertices, 3, NULL, 0);
                }
            }
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
    matrix->m11 = -1; // Part in making y>0 up
    matrix->m22 = 1;
    matrix->m33 = 1;
}

void set_frustum_matrix(Matrix4x4 *matrix, float near, float far, float left, float right, float bottom, float top)
{
    matrix->m00 = (2 * near) / (right - left);
    matrix->m02 = (right + left) / (right - left);
    matrix->m11 = -(2 * near) / (top - bottom);
    matrix->m12 = (top + bottom) / (top - bottom);
    matrix->m22 = -(far + near) / (far - near); // this has '-' to help make y>0 up
    matrix->m23 = -(2 * far * near) / (far - near);
    matrix->m32 = -1;
}

void set_x_rotation_matrix(Matrix4x4 *matrix, float angle)
{
    matrix->m00 = 1;
    matrix->m11 = cos(angle);
    matrix->m12 = -sin(angle);
    matrix->m21 = sin(angle);
    matrix->m22 = cos(angle);
    matrix->m33 = 1;
}

void set_y_rotation_matrix(Matrix4x4 *matrix, float angle)
{
    matrix->m00 = cos(angle);
    matrix->m02 = sin(angle);
    matrix->m11 = 1;
    matrix->m20 = -sin(angle);
    matrix->m22 = cos(angle);
    matrix->m33 = 1;
}

void set_z_rotation_matrix(Matrix4x4 *matrix, float angle)
{
    matrix->m00 = cos(angle);
    matrix->m01 = -sin(angle);
    matrix->m10 = sin(angle);
    matrix->m11 = cos(angle);
    matrix->m22 = 1;
    matrix->m33 = 1;
}

void apply_position(Vector3f position, Matrix4x4 *camera_matrix)
{
    camera_matrix->m30 = position.x;
    camera_matrix->m31 = position.y;
    camera_matrix->m32 = position.z;
}
