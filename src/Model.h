#ifndef MODEL_H
#define MODEL_H

#define MODEL_SIZE 80000 
#define FACES_SIZE (int)(MODEL_SIZE * 2)

#include "Vector.h"


#include "Matrix.h"


#include <SDL2/SDL_render.h>

typedef struct{
    Vector3f *vertices;
    Vector3f *normals;

    Vector3 **faces;

    // number of vertices in face
    // index in array is index of face
    int *face_data;
    float *vertex_scale;

    SDL_Vertex screen_vertices[MODEL_SIZE];
    Vector3f screen[MODEL_SIZE];

    Vector2f *ordered_faces;

    Vector3f position;
    Vector3f rotation;

    int vertex_count;
    int face_count;
} Model;


void load_model(Model *model, const char* filepath);

void calculate_vertices(Model *model, Matrix4x4 *transform_matrix, Matrix4x4 *projection_matrix);
void sort_faces(Model *model);
void draw_model(Model *model, SDL_Renderer *renderer);

#endif // MODEL_H
