#include <math.h>

#include "types.h"

void rotate(Point *source, Point *target, s32 angle) {
  s32 s = (s32)(sin(angle / 256.0) * 256.0);
  s32 c = (s32)(cos(angle / 256.0) * 256.0);

  target->x = ((source->x * c) >> 8) - ((source->y * s) >> 8);
  target->y = ((source->x * s) >> 8) + ((source->y * c) >> 8);
}

void project(Point *source, Point *target) {
  target->x = source->x + (SCREEN_WIDTH << 7);
  target->y = -source->y + (SCREEN_HEIGHT << 7);
}
