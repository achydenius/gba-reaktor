#include <gba_video.h>

#include "math.h"
#include "objects.h"
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
  for (u32 i = 0; i < 256; i++) {
    BG_PALETTE[i] = palette[i];
  }

  REG_DISPCNT = MODE_4 | BG2_ON;

  object.vertices[0].color = 31 << 8;
  object.vertices[1].color = 31 << 8;
  object.vertices[2].color = 255 << 8;
  object.vertices[3].color = 255 << 8;
  object.vertices[4].color = 31 << 8;
  object.vertices[5].color = 31 << 8;
  object.vertices[6].color = 255 << 8;
  object.vertices[7].color = 255 << 8;

  u32 angle = 0;
  while (1) {
    wait_vblank();

    clear_screen(4);

    u32 trig_table_wrap = TRIG_TABLE_SIZE - 1;

    vector_set(&object.rotation, 0, angle & trig_table_wrap, angle & trig_table_wrap);
    vector_set(&object.translation, sin_table[angle & trig_table_wrap] << 5, 0, -80 << 8);
    object_render(&object);

    angle++;

    swap_buffers();
  }
  return 0;
}
