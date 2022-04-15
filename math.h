#include <gba_base.h>

#include "tables.h"
#include "types.h"

void create_rotation_matrix(Matrix *matrix, s32 x, s32 y, s32 z) {
  s32 sin_x = sin_table[x];
  s32 cos_x = cos_table[x];
  s32 sin_y = sin_table[y];
  s32 cos_y = cos_table[y];
  s32 sin_z = sin_table[z];
  s32 cos_z = cos_table[z];

  s32 cos_z_sin_y = cos_z * sin_y;
  s32 sin_z_sin_y = sin_z * sin_y;

  matrix->m00 = (cos_z * cos_y) >> 8;
  matrix->m01 = (sin_z * cos_y) >> 8;
  matrix->m02 = -sin_y;

  matrix->m10 = ((cos_z_sin_y * sin_x) >> 16) - ((sin_z * cos_x) >> 8);
  matrix->m11 = ((sin_z_sin_y * sin_x) >> 16) + ((cos_z * cos_x) >> 8);
  matrix->m12 = (cos_y * sin_x) >> 8;

  matrix->m20 = ((cos_z_sin_y * cos_x) >> 16) + ((sin_z * sin_x) >> 8);
  matrix->m21 = ((sin_z_sin_y * cos_x) >> 16) - ((cos_z * sin_x) >> 8);
  matrix->m22 = (cos_y * cos_x) >> 8;
}

void transform_vector(Vector3D *source, Matrix *matrix, Vector3D *target) {
  target->x = (source->x * matrix->m00 + source->y * matrix->m01 + source->z * matrix->m02) >> 8;
  target->y = (source->x * matrix->m10 + source->y * matrix->m11 + source->z * matrix->m12) >> 8;
  target->z = (source->x * matrix->m20 + source->y * matrix->m21 + source->z * matrix->m22) >> 8;
}

void project_vector(Vector3D *source, s32 camera_distance, s32 projection_distance,
                    Vector2D *target) {
  s32 z = source->z + camera_distance;
  s32 div = (projection_distance << 16) / z;
  target->x = ((source->x * div) >> 16) + (SCREEN_WIDTH << 7);
  target->y = ((-source->y * div) >> 16) + (SCREEN_HEIGHT << 7);
}
