#include <gba_base.h>

#include "tables.h"
#include "types.h"

void matrix_rotation(Matrix *matrix, s32 x, s32 y, s32 z) {
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
  matrix->m03 = 0;

  matrix->m10 = ((cos_z_sin_y * sin_x) >> 16) - ((sin_z * cos_x) >> 8);
  matrix->m11 = ((sin_z_sin_y * sin_x) >> 16) + ((cos_z * cos_x) >> 8);
  matrix->m12 = (cos_y * sin_x) >> 8;
  matrix->m13 = 0;

  matrix->m20 = ((cos_z_sin_y * cos_x) >> 16) + ((sin_z * sin_x) >> 8);
  matrix->m21 = ((sin_z_sin_y * cos_x) >> 16) - ((cos_z * sin_x) >> 8);
  matrix->m22 = (cos_y * cos_x) >> 8;
  matrix->m23 = 0;
}

void matrix_translation(Matrix *matrix, s32 x, s32 y, s32 z) {
  matrix->m00 = 1 << 8;
  matrix->m01 = 0;
  matrix->m02 = 0;
  matrix->m03 = x;

  matrix->m10 = 0;
  matrix->m11 = 1 << 8;
  matrix->m12 = 0;
  matrix->m13 = y;

  matrix->m20 = 0;
  matrix->m21 = 0;
  matrix->m22 = 1 << 8;
  matrix->m23 = z;
}

void matrix_multiply(Matrix *a, Matrix *b, Matrix *result) {
  result->m00 = (a->m00 * b->m00 + a->m01 * b->m10 + a->m02 * b->m20) >> 8;
  result->m01 = (a->m00 * b->m01 + a->m01 * b->m11 + a->m02 * b->m21) >> 8;
  result->m02 = (a->m00 * b->m02 + a->m01 * b->m12 + a->m02 * b->m22) >> 8;
  result->m03 = ((a->m00 * b->m03 + a->m01 * b->m13 + a->m02 * b->m23) >> 8) + a->m03;

  result->m10 = (a->m10 * b->m00 + a->m11 * b->m10 + a->m12 * b->m20) >> 8;
  result->m11 = (a->m10 * b->m01 + a->m11 * b->m11 + a->m12 * b->m21) >> 8;
  result->m12 = (a->m10 * b->m02 + a->m11 * b->m12 + a->m12 * b->m22) >> 8;
  result->m13 = ((a->m10 * b->m03 + a->m11 * b->m13 + a->m12 * b->m23) >> 8) + a->m13;

  result->m20 = (a->m20 * b->m00 + a->m21 * b->m10 + a->m22 * b->m20) >> 8;
  result->m21 = (a->m20 * b->m01 + a->m21 * b->m11 + a->m22 * b->m21) >> 8;
  result->m22 = (a->m20 * b->m02 + a->m21 * b->m12 + a->m22 * b->m22) >> 8;
  result->m23 = ((a->m20 * b->m03 + a->m21 * b->m13 + a->m22 * b->m23) >> 8) + a->m23;
}

void matrix_transpose_rotation(Matrix *source, Matrix *target) {
  target->m00 = source->m00;
  target->m01 = source->m10;
  target->m02 = source->m20;
  target->m03 = 0;

  target->m10 = source->m01;
  target->m11 = source->m11;
  target->m12 = source->m21;
  target->m13 = 0;

  target->m20 = source->m02;
  target->m21 = source->m12;
  target->m22 = source->m22;
  target->m23 = 0;
}

void vector_set(Vector3D *vector, s32 x, s32 y, s32 z) {
  vector->x = x;
  vector->y = y;
  vector->z = z;
}

void vector_multiply(Vector3D *source, Matrix *matrix, Vector3D *target) {
  target->x = ((source->x * matrix->m00 + source->y * matrix->m01 + source->z * matrix->m02) >> 8) +
              matrix->m03;
  target->y = ((source->x * matrix->m10 + source->y * matrix->m11 + source->z * matrix->m12) >> 8) +
              matrix->m13;
  target->z = ((source->x * matrix->m20 + source->y * matrix->m21 + source->z * matrix->m22) >> 8) +
              matrix->m23;
}

void vector_project(Vector3D *source, s32 projection_distance, Vector2D *target) {
  s32 div = (projection_distance << 16) / source->z;
  target->x = ((-source->x * div) >> 16) + (SCREEN_WIDTH << 7);
  target->y = ((source->y * div) >> 16) + (SCREEN_HEIGHT << 7);
}

s32 vector_dot(Vector3D *a, Vector3D *b) {
  return ((a->x * b->x) + (a->y * b->y) + (a->z * b->z)) >> 8;
}

void vector_subtract(Vector2D *a, Vector2D *b, Vector2D *result) {
  result->x = a->x - b->x;
  result->y = a->y - b->y;
}
