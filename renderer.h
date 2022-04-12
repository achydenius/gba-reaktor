#include <gba_base.h>

u16* g_buffer = (u16*)MODE5_BB;

typedef struct Point {
  s32 x, y;
} Point;

static void swap(u16* a, u16* b) {
  u16 tmp = *a;
  *a = *b;
  *b = tmp;
}

void clear_screen(u8 color) {
  u32 value = (color << 24) | (color << 16) | (color << 8) | color;

  for (u32 i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT / 4; i++) {
    ((u32*)g_buffer)[i] = value;
  }
}

void put_pixel(u8 x, u8 y, u8 color) {
  u32 index = (y * SCREEN_WIDTH / 2) + (x / 2);
  u16 pair = g_buffer[index];

  if (x & 1) {
    g_buffer[index] = (pair & 0xFF) | (color << 8);
  } else {
    g_buffer[index] = (pair & 0xFF00) | color;
  }
}

void trace_edge(u16 x0, u16 y0, u16 x1, u16 y1, u8 color) {
  if (y0 > y1) {
    swap(&x0, &x1);
    swap(&y0, &y1);
  }

  s32 dx = ((x1 - x0) << 8) / (y1 - y0);
  s32 ex = (1 << 8) - (y0 & 0xff);
  s32 x = x0 + ((dx * ex) >> 8);

  for (u32 y = y0 >> 8; y < y1 >> 8; y++) {
    put_pixel(x >> 8, y, color);
    x += dx;
  }
}

void draw_polygon(Point* points, u32 point_count, u8 color) {
  for (u32 i = 0; i < point_count; i++) {
    Point* a = &points[i];
    Point* b = &points[(i + 1) % point_count];

    trace_edge(a->x, a->y, b->x, b->y, color);
  }
}
