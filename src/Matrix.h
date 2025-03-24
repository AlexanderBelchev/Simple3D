#ifndef MATRIX_H
#define MATRIX_H

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

void MulVec3By4x4(Vector3f *vec, Matrix4x4 *mat, Vector3f *out);
void mulVec3By4x4(Vector3f *vec, Matrix3x3 *mat, Vector3f *out);


void SetXRotationMatrix(Matrix4x4 *matrix, float angle);
void SetYRotationMatrix(Matrix4x4 *matrix, float angle);
void SetZRotationMatrix(Matrix4x4 *matrix, float angle);

void SetFrustumMatrix(Matrix4x4 *matrix, float near, float far, float left, float right, float bottom, float top);
void SetTransformMatrix(Matrix4x4 *matrix, Vector3f position);

#endif // MATRIX_H
