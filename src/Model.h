#define MODEL_SIZE 80000 
#define FACES_SIZE (int)(MODEL_SIZE * 2)

#include "Vector.h"

typedef struct{
    Vector3f vertices[MODEL_SIZE];
    int faces[FACES_SIZE][5];
    Vector2f screen_points[MODEL_SIZE];
    int vertex_count;
    int face_count;
} Model;


void load_model(Model *model, const char* filepath);
