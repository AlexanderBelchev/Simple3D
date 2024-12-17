#include "Camera.h"
#include <math.h>

Matrix4x4 projection_matrix;
float ratio, fov, near, far, bottom, top, left, right;

void InitializeCamera(int screen_width, int screen_height, float camera_fov, float near_plane, float far_plane)
{
    projection_matrix = (Matrix4x4){0};
    fov = camera_fov;
    near = near_plane;
    far = far_plane;
    ratio = (float)screen_width/(float)screen_height;
    top = tan(fov/2) * near_plane;
    bottom = -top;
    right = top * ratio;
    left = bottom * ratio;
}
