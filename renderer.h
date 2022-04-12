#include <gba_base.h>

u16* g_buffer = (u16*)MODE5_BB;

static u8 left_edges[SCREEN_HEIGHT];
static u8 right_edges[SCREEN_HEIGHT];
static u8 top_edge;
static u8 bottom_edge;

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

void trace_edge(u16 x0, u16 y0, u16 x1, u16 y1, u8* buffer) {
  if (y0 > y1) {
    swap(&x0, &x1);
    swap(&y0, &y1);
  }

  u8 start_y = y0 >> 8;
  u8 end_y = y1 >> 8;

  if (start_y < top_edge) {
    top_edge = start_y;
  }
  if (end_y > bottom_edge) {
    bottom_edge = end_y;
  }

  s32 dx = ((x1 - x0) << 8) / (y1 - y0);
  s32 ex = (1 << 8) - (y0 & 0xFF);
  s32 x = x0 + ((dx * ex) >> 8);

  for (u32 y = start_y; y < end_y; y++) {
    buffer[y] = x >> 8;
    x += dx;
  }
}

void draw_polygon(Point* points, u32 point_count, u8 color) {
  top_edge = 255;
  bottom_edge = 0;

  for (u32 i = 0; i < point_count; i++) {
    Point* a = &points[i];
    Point* b = &points[(i + 1) % point_count];

    if (a->y < b->y) {
      trace_edge(a->x, a->y, b->x, b->y, left_edges);
    } else {
      trace_edge(a->x, a->y, b->x, b->y, right_edges);
    }
  }

  for (u32 y = top_edge; y < bottom_edge; y++) {
    for (u8 x = left_edges[y]; x < right_edges[y]; x++) {
      put_pixel(x, y, 1);
    }
  }
}
