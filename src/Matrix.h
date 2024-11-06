#include "Vector.h"

typedef struct
{
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;
} Matrix4x4;

typedef struct
{
    float m00, m01, m02;
    float m10, m11, m12;
    float m20, m21, m22;
} Matrix3x3;

typedef struct
{
    float x, y, z, w;
} PointMatrix;

void MultiplyPointBy4x4(Vector3f *vec, Matrix4x4 *mat, Vector3f *out);
void MultiplyPointBy3x3(Vector3f *vec, Matrix3x3 *mat, Vector3f *out);
