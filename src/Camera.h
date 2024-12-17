#ifndef CAMERA_H
#define CAMERA_H

#include "Matrix.h"


extern Matrix4x4 projection_matrix;
extern float ratio, fov, near, far, bottom, top, left, right;
void InitializeCamera(int screen_width, int screen_height, float camera_fov, float near_plane, float far_plane);

#endif // CAMERA_H
