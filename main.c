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
  BG_PALETTE[0] = 0;
  BG_PALETTE[1] = RGB5(0x1F, 0x1F, 0x1F);
  BG_PALETTE[2] = RGB5(0x1F, 0, 0);
  BG_PALETTE[3] = RGB5(0, 0x1F, 0);
  BG_PALETTE[4] = RGB5(0, 0, 0x1F);

  REG_DISPCNT = MODE_4 | BG2_ON;

  object.polygons[0].color = 1;
  object.polygons[1].color = 2;
  object.polygons[2].color = 1;
  object.polygons[3].color = 2;
  object.polygons[4].color = 3;
  object.polygons[5].color = 3;

  u32 angle = 0;
  while (1) {
    wait_vblank();

    clear_screen(4);

    u32 trig_table_wrap = TRIG_TABLE_SIZE - 1;

    vector_set(&object.rotation, 0, (angle >> 1) & trig_table_wrap, angle & trig_table_wrap);
    vector_set(&object.translation, sin_table[angle & trig_table_wrap] << 5, 0, 80 << 8);
    object_render(&object);

    angle++;

    swap_buffers();
  }
  return 0;
}
