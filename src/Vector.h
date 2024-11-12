#ifndef VECTOR_H
#define VECTOR_H

typedef struct
{
    int x, y, z;
} Vector3;
typedef struct
{
    float x, y, z;
} Vector3f;

typedef struct
{
    float x, y;
} Vector2f;

Vector3f Scalef(Vector3f vector_a, Vector3f vector_b);
int Cross(Vector2f a, Vector2f b); 
void Sub(Vector2f a, Vector2f b, Vector2f *out);

#endif /* VECTOR_H */
