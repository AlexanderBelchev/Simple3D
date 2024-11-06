#include "Vector.h"

Vector3f VectorScale(Vector3f vector_a, Vector3f vector_b)
{
    Vector3f vector_scaled;
    vector_scaled.x = vector_a.x * vector_b.x;
    vector_scaled.y = vector_a.y * vector_b.y;
    vector_scaled.z = vector_a.z * vector_b.z;
    return vector_scaled;
}


