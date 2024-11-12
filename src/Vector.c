#include "Vector.h"

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
