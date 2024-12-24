#ifndef MODEL_H
#define MODEL_H

#define MODEL_SIZE 80000 
#define FACES_SIZE (int)(MODEL_SIZE * 2)

#include "Vector.h"
#include "Matrix.h"

#include <SDL2/SDL_render.h>

typedef struct
{
    // Array of vectors, representing the positions of vertices
    Vector3f *vertices;

    // Array of vectors, representing the position of vertices, with applied rotation
    Vector3f *rotated_vertices;

    /* Vector array containing faces and their data as a vector [FACES_SIZE][n] where
     * n is the numbert of vertices in the face
     * faces[i][j] - i is the index of the face, j is the index/order of the vertex in the face
     * faces[i][j].x - Vertex index from *vertices
     * faces[i][j].y - Texture vertices -- TODO: not yet implemented
     * faces[i][j].z - Vertex normal -- TODO: not really needed as it is calculated on runtime
     */
    Vector3 **faces;

    // Array that stores the number of vertices in a face.
    // The index i in this array is the index i of the face in **faces
    int *face_data;
    Vector3f *face_normal;
    
    // This is used when drawing -- TODO: just a testing variable
    float *face_scale;

    // SDL_Vertex points, used when drawing faces to the screen. Projected screen points.
    SDL_Vertex screen_vertices[MODEL_SIZE];
    Vector3f screen[MODEL_SIZE];

    // Faces draw order is stored here
    // x - index of face (has to be converted to int)
    // y - average depth of all vertecies in the face
    Vector2f *ordered_faces;

    // Position of the model in world space
    Vector3f position;

    // Rotation of the model
    Vector3f rotation;

    int vertex_count;
    int face_count;
} Model;


void load_model(Model *model, const char* filepath);

void calculate_vertices(Model *model, Matrix4x4 *transform_matrix, Matrix4x4 *projection_matrix);
void sort_faces(Model *model);
void draw_model(Model *model, SDL_Renderer *renderer);

#endif // MODEL_H
