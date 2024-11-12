#ifndef VECTOR_H
#define VECTOR_H

typedef struct
{
    float x, y, z;
} Vector3f;

typedef struct
{
    float x, y;
} Vector2f;

Vector3f Scale(Vector3f vector_a, Vector3f vector_b);

#endif /* VECTOR_H */
