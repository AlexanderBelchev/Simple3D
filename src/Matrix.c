#include "Matrix.h"
#include <stdio.h>
#include <math.h>

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

void SetXRotationMatrix(Matrix4x4 *matrix, float angle)
{
    matrix->m00 = 1;
    matrix->m11 = cos(angle);
    matrix->m12 = -sin(angle);
    matrix->m21 = sin(angle);
    matrix->m22 = cos(angle);
    matrix->m33 = 1;
}

void SetYRotationMatrix(Matrix4x4 *matrix, float angle)
{
    matrix->m00 = cos(angle);
    matrix->m02 = sin(angle);
    matrix->m11 = 1;
    matrix->m20 = -sin(angle);
    matrix->m22 = cos(angle);
    matrix->m33 = 1;
}

void SetZRotationMatrix(Matrix4x4 *matrix, float angle)
{
    matrix->m00 = cos(angle);
    matrix->m01 = -sin(angle);
    matrix->m10 = sin(angle);
    matrix->m11 = cos(angle);
    matrix->m22 = 1;
    matrix->m33 = 1;
}


void SetFrustumMatrix(Matrix4x4 *matrix, float near, float far, float left, float right, float bottom, float top)
{
    matrix->m00 = (2 * near) / (right - left);
    matrix->m02 = (right + left) / (right - left);
    matrix->m11 = -(2 * near) / (top - bottom);
    matrix->m12 = (top + bottom) / (top - bottom);
    matrix->m22 = -(far + near) / (far - near);
    matrix->m23 = -(2 * far * near) / (far - near);
    matrix->m32 = -1;
}

void SetTransformMatrix(Matrix4x4 *matrix, Vector3f position)
{
    matrix->m00 = 1;
    matrix->m11 = 1;
    matrix->m22 = 1;
    matrix->m30 = position.x;
    matrix->m31 = position.y;
    matrix->m32 = position.z;
    matrix->m33 = 1;
}
