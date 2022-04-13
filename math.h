#include "tables.h"
#include "types.h"

void rotate(Point *source, Point *target, s32 angle) {
  s32 s = sin_table[angle];
  s32 c = cos_table[angle];

  target->x = ((source->x * c) >> 8) - ((source->y * s) >> 8);
  target->y = ((source->x * s) >> 8) + ((source->y * c) >> 8);
}

void project(Point *source, Point *target) {
  target->x = source->x + (SCREEN_WIDTH << 7);
  target->y = -source->y + (SCREEN_HEIGHT << 7);
}
