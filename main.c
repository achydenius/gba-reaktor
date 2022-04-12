#include <gba_video.h>

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

  while (1) {
    wait_vblank();

    clear_screen(4);

    Point points[] = {{70 << 8, 40 << 8}, {90 << 8, 120 << 8}, {170 << 8, 65 << 8}};
    draw_polygon(points, 3, 1);

    swap_buffers();
  }
  return 0;
}
