#include "types.h"

u16* g_buffer = (u16*)MODE5_BB;

typedef struct Edge {
  s32 x, c;
} Edge;

Edge left_edges[SCREEN_HEIGHT];
Edge right_edges[SCREEN_HEIGHT];
u32 top_edge;
u32 bottom_edge;

void swap(s32* a, s32* b) {
  s32 tmp = *a;
  *a = *b;
  *b = tmp;
}

void clear_screen(u32 color) {
  u32 value = (color << 24) | (color << 16) | (color << 8) | color;

  for (u32 i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT / 4; i++) {
    ((u32*)g_buffer)[i] = value;
  }
}

void put_pixel(u32 x, u32 y, u32 color) {
  u32 index = (y * SCREEN_WIDTH / 2) + (x / 2);
  u32 pair = g_buffer[index];

  if (x & 1) {
    g_buffer[index] = (pair & 0xFF) | (color << 8);
  } else {
    g_buffer[index] = (pair & 0xFF00) | color;
  }
}

void trace_edge(s32 x0, s32 y0, s32 c0, s32 x1, s32 y1, s32 c1, Edge* buffer) {
  if (y0 > y1) {
    swap(&x0, &x1);
    swap(&y0, &y1);
    swap(&c0, &c1);
  }

  u32 start_y = y0 >> 8;
  u32 end_y = y1 >> 8;

  if (start_y < top_edge) {
    top_edge = start_y;
  }
  if (end_y > bottom_edge) {
    bottom_edge = end_y;
  }

  s32 dx = ((x1 - x0) << 8) / (y1 - y0);
  s32 ex = (1 << 8) - (y0 & 0xFF);
  u32 x = x0 + ((dx * ex) >> 8);

  s32 dc = ((c1 - c0) << 8) / (y1 - y0);
  s32 ec = (1 << 8) - (y0 & 0xFF);
  s32 c = c0 + ((dc * ec) >> 8);

  for (u32 y = start_y; y < end_y; y++) {
    buffer[y].x = x;
    buffer[y].c = c;
    x += dx;
    c += dc;
  }
}

void draw_polygon(Polygon* polygon) {
  top_edge = 255;
  bottom_edge = 0;

  for (u32 i = 0; i < polygon->vertex_count; i++) {
    Vertex* a = polygon->vertices[i];
    Vertex* b = polygon->vertices[(i + 1) % polygon->vertex_count];
    Vector2D* ap = &a->projected;
    Vector2D* bp = &b->projected;

    if (ap->y < bp->y) {
      trace_edge(ap->x, ap->y, a->color, bp->x, bp->y, b->color, left_edges);
    } else {
      trace_edge(ap->x, ap->y, a->color, bp->x, bp->y, b->color, right_edges);
    }
  }

  for (u32 y = top_edge; y < bottom_edge; y++) {
    s32 start_x = left_edges[y].x;
    s32 end_x = right_edges[y].x;
    s32 start_c = left_edges[y].c;
    s32 end_c = right_edges[y].c;

    s32 dc = ((end_c - start_c) << 8) / (end_x - start_x);
    s32 c = left_edges[y].c;

    for (u32 x = start_x >> 8; x < end_x >> 8; x++) {
      put_pixel(x, y, c >> 8);
      c += dc;
    }
  }
}
