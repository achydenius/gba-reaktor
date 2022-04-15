#ifndef __TYPES_H__
#define __TYPES_H__

#include <gba_base.h>

typedef struct Vector2D {
  s32 x, y;
} Vector2D;

typedef struct Vector3D {
  s32 x, y, z;
} Vector3D;

typedef struct Matrix {
  s32 m00, m01, m02;
  s32 m10, m11, m12;
  s32 m20, m21, m22;
} Matrix;

#endif
