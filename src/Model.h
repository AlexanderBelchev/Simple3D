#define MODEL_SIZE 80000 
#define FACES_SIZE (int)(MODEL_SIZE * 2)

#include "Vector.h"

#include <SDL2/SDL_render.h>

typedef struct{
    Vector3f *vertices;
    Vector3f *normals;

    Vector3 **faces;

    // number of vertices in face
    // index in array is index of face
    int *face_data;

    SDL_Vertex screen_vertices[MODEL_SIZE];
    Vector3f screen[MODEL_SIZE];

    int vertex_count;
    int face_count;
} Model;


void load_model(Model *model, const char* filepath);
