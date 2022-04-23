#include "rasterizer.h"

Polygon *visible_polygons[60];

void sort_polygons(Polygon *polygons[], u32 count) {
  Polygon *current;

  for (int i = 1; i < count; i++) {
    current = polygons[i];
    int j = i - 1;

    while (j >= 0 && polygons[j]->z > current->z) {
      polygons[j + 1] = polygons[j];
      j = j - 1;
    }
    polygons[j + 1] = current;
  }
}

bool polygon_is_visible(Polygon *polygon) {
  Vector2D a, b;
  vector_subtract(&polygon->vertices[2]->projected, &polygon->vertices[0]->projected, &a);
  vector_subtract(&polygon->vertices[1]->projected, &polygon->vertices[0]->projected, &b);

  return (a.x * b.y) - (a.y * b.x) > 0;
}

void object_render(Object *object, s32 camera_distance) {
  Matrix rotation, inverse_rotation;

  matrix_rotation(&rotation, object->rotation.x, object->rotation.y, object->rotation.z);
  matrix_transpose(&rotation, &inverse_rotation);

  Vector3D light = {0, 0, 255 << 8};
  Vector3D light_transformed;
  vector_multiply(&light, &inverse_rotation, &light_transformed);

  for (u32 i = 0; i < object->vertex_count; i++) {
    Vertex *vertex = &object->vertices[i];
    vector_multiply(&vertex->original, &rotation, &vertex->transformed);
    vector_project(&vertex->transformed, 100 << 8, camera_distance, &vertex->projected);
  }

  u32 visible_polygon_count = 0;
  for (u32 i = 0; i < object->polygon_count; i++) {
    Polygon *polygon = &object->polygons[i];
    if (polygon_is_visible(polygon)) {
      u32 z_sum = 0;
      for (u32 j = 0; j < polygon->vertex_count; j++) {
        z_sum += polygon->vertices[j]->transformed.z + camera_distance;
      }
      polygon->z = (z_sum << 8) / polygon->vertex_count;
      visible_polygons[visible_polygon_count++] = polygon;
    }
  }

  sort_polygons(visible_polygons, visible_polygon_count);

  for (u32 i = 0; i < visible_polygon_count; i++) {
    Polygon *polygon = visible_polygons[i];
    if (polygon_is_visible(polygon)) {
      s32 color = vector_dot(&polygon->normal, &light_transformed);
      polygon->color = color > 0 ? color >> 8 : 0;

      draw_polygon(polygon);
    }
  }
}
