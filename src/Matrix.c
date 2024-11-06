#include "Matrix.h"
#include <stdio.h>

void MultiplyPointBy4x4(Vector3f *vec, Matrix4x4 *mat, Vector3f *out)
{
    out->x = vec->x * mat->m00 + vec->y * mat->m10 + vec->z * mat->m20 + mat->m30;
    out->y = vec->x * mat->m01 + vec->y * mat->m11 + vec->z * mat->m21 + mat->m31;
    out->z = vec->x * mat->m02 + vec->y * mat->m12 + vec->z * mat->m22 + mat->m32;
    float w = vec->x * mat->m03 * vec->y * mat->m13 + vec->z * mat->m23 +mat->m33;
    if(w != 1)
    {
        out->x /= w;
        out->y /= w;
        out->z /= w;
    }
}

void MultiplyPointBy3x3(Vector3f *vec, Matrix3x3 *mat, Vector3f *out)
{
    out->x = vec->x * mat->m00 + vec->y * mat->m10 + vec->z * mat->m20;
    out->y = vec->x * mat->m01 + vec->y * mat->m11 + vec->z * mat->m21;
    out->z = vec->x * mat->m02 + vec->y * mat->m12 + vec->z * mat->m22;
}
