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
  s32 m00, m01, m02, m03;
  s32 m10, m11, m12, m13;
  s32 m20, m21, m22, m23;
} Matrix;

typedef struct Vertex {
  Vector3D original, transformed;
  Vector2D projected;
} Vertex;

typedef struct Polygon {
  Vertex **vertices;
  u32 vertex_count;
  Vector3D normal;
  u32 color;
  s32 z;
} Polygon;

typedef struct Object {
  Vertex *vertices;
  Polygon *polygons;
  u32 vertex_count;
  u32 polygon_count;
  Vector3D rotation, translation;
} Object;

#endif
