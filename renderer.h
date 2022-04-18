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

void object_render(Object *object) {
  Matrix rotation, translation, model_to_world, inverse_rotation;

  // Create model to world space transformation matrix
  matrix_rotation(&rotation, object->rotation.x, object->rotation.y, object->rotation.z);
  matrix_translation(&translation, object->translation.x, object->translation.y,
                     object->translation.z);
  matrix_multiply(&translation, &rotation, &model_to_world);

  // Create inverse rotation matrix for lighting
  matrix_transpose_rotation(&rotation, &inverse_rotation);

  Vector3D light = {0, 0, 255 << 8};
  Vector3D light_transformed;
  vector_multiply(&light, &inverse_rotation, &light_transformed);

  for (u32 i = 0; i < object->vertex_count; i++) {
    Vertex *vertex = &object->vertices[i];
    vector_multiply(&vertex->original, &model_to_world, &vertex->transformed);
    vector_project(&vertex->transformed, 100 << 8, &vertex->projected);
  }

  u32 visible_polygon_count = 0;
  for (u32 i = 0; i < object->polygon_count; i++) {
    Polygon *polygon = &object->polygons[i];
    if (polygon_is_visible(polygon)) {
      u32 z_sum = 0;
      for (u32 j = 0; j < polygon->vertex_count; j++) {
        z_sum += polygon->vertices[j]->transformed.z;
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
