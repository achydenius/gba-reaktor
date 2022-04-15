#include "rasterizer.h"

void object_render(Object *object) {
  Matrix rotation, translation, matrix;

  matrix_rotation(&rotation, object->rotation.x, object->rotation.y, object->rotation.z);
  matrix_translation(&translation, object->translation.x, object->translation.y,
                     object->translation.z);
  matrix_multiply(&translation, &rotation, &matrix);

  for (u32 i = 0; i < object->vertex_count; i++) {
    Vertex *vertex = &object->vertices[i];
    vector_multiply(&vertex->original, &matrix, &vertex->transformed);
    vector_project(&vertex->transformed, 100 << 8, &vertex->projected);
  }

  for (u32 i = 0; i < object->polygon_count; i++) {
    draw_polygon(&object->polygons[i]);
  }
}
