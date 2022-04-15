#include <gba_video.h>

#include "math.h"
#include "renderer.h"

void wait_vblank() {
  while (REG_VCOUNT < SCREEN_HEIGHT) {
  }
}

void swap_buffers() {
  REG_DISPCNT ^= BACKBUFFER;
  g_buffer = REG_DISPCNT & BACKBUFFER ? (u16*)MODE5_FB : (u16*)MODE5_BB;
}

int main() {
  BG_PALETTE[0] = 0;
  BG_PALETTE[1] = RGB5(0x1F, 0x1F, 0x1F);
  BG_PALETTE[2] = RGB5(0x1F, 0, 0);
  BG_PALETTE[3] = RGB5(0, 0x1F, 0);
  BG_PALETTE[4] = RGB5(0, 0, 0x1F);

  REG_DISPCNT = MODE_4 | BG2_ON;

  Vertex vertices[] = {{{-40 << 8, 40 << 8, 0}},  {{-40 << 8, 20 << 8, 0}},
                       {{40 << 8, 20 << 8, 0}},   {{40 << 8, 40 << 8, 0}},
                       {{-40 << 8, -20 << 8, 0}}, {{-40 << 8, -40 << 8, 0}},
                       {{40 << 8, -40 << 8, 0}},  {{40 << 8, -20 << 8, 0}}};
  Polygon polygons[] = {
      {(Vertex*[]){&vertices[0], &vertices[1], &vertices[2], &vertices[3]}, 4, 1},
      {(Vertex*[]){&vertices[4], &vertices[5], &vertices[6], &vertices[7]}, 4, 1},
  };
  Object object = {vertices, polygons, 8, 2};

  u32 angle = 0;
  while (1) {
    wait_vblank();

    clear_screen(4);

    vector_set(&object.rotation, 0, angle >> 1, angle);
    vector_set(&object.translation, sin_table[angle] << 6, 0, 80 << 8);
    object_render(&object);

    angle = (angle + 1) & (TRIG_TABLE_SIZE - 1);

    swap_buffers();
  }
  return 0;
}
