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
void Sub3f(Vector3f a, Vector3f b, Vector3f *out);
void Normalize3f(Vector3f a, Vector3f *out);
float Magnitude3f(Vector3f a);
float Dot3f(Vector3f a, Vector3f b);
// Returns degrees
float Angle3f(Vector3f a, Vector3f b);

void Multiply3f(Vector3f a, Vector3f b, Vector3f *out);
void Cross3f(Vector3f u, Vector3f v, Vector3f *out);

#endif /* VECTOR_H */
