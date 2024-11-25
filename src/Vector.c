#include "Vector.h"

#include <math.h>

Vector3f Scalef(Vector3f vector_a, Vector3f vector_b)
{
    Vector3f vector_scaled;
    vector_scaled.x = vector_a.x * vector_b.x;
    vector_scaled.y = vector_a.y * vector_b.y;
    vector_scaled.z = vector_a.z * vector_b.z;
    return vector_scaled;
}

int Cross(Vector2f a, Vector2f b)
{
    return -a.x * b.y + a.y * b.x;
}

void Sub(Vector2f a, Vector2f b, Vector2f *out)
{
    out->x = a.x - b.x;
    out->y = a.y - b.y;
}

void Sub3f(Vector3f a, Vector3f b, Vector3f *out)
{
    out->x = a.x - b.x;
    out->y = a.y - b.y;
    out->z = a.z - b.z;
}

void Normalize3f(Vector3f a, Vector3f *out)
{
    float mag = Magnitude3f(a);
    out->x = a.x / mag;
    out->y = a.y / mag;
    out->z = a.z / mag;
}

float Magnitude3f(Vector3f a)
{
    return sqrt((a.x*a.x) + (a.y*a.y) + (a.z*a.z));
}

float Dot3f(Vector3f a, Vector3f b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

//float Angle3f(
