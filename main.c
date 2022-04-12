#include <gba_video.h>

u16* buffer = (u16*)MODE5_BB;

void wait_vblank() {
  while (REG_VCOUNT < SCREEN_HEIGHT) {
  }
}

void clear_screen(u8 color) {
  u32 value = (color << 24) | (color << 16) | (color << 8) | color;
  u32* buf = (u32*)buffer;
  u32 i = SCREEN_WIDTH * SCREEN_HEIGHT / 4;
  while (i > 0) {
    *(buf++) = value;
    i--;
  }
}

void swap_buffers() {
  REG_DISPCNT ^= BACKBUFFER;
  buffer = REG_DISPCNT & BACKBUFFER ? (u16*)MODE5_FB : (u16*)MODE5_BB;
}

void put_pixel(u8 x, u8 y, u8 color) {
  u16* buf = &buffer[(y * SCREEN_WIDTH / 2) + (x / 2)];
  *buf = x & 1 ? ((*buf & 0xFF) | (color << 8)) : ((*buf & 0xFF00) | color);
}

int main() {
  BG_PALETTE[0] = 0;
  BG_PALETTE[1] = RGB5(0x1F, 0x1F, 0x1F);
  BG_PALETTE[2] = RGB5(0x1F, 0, 0);
  BG_PALETTE[3] = RGB5(0, 0x1F, 0);
  BG_PALETTE[4] = RGB5(0, 0, 0x1F);

  REG_DISPCNT = MODE_4 | BG2_ON;

  u32 x = 0;
  s32 dir = 1;

  while (1) {
    wait_vblank();

    clear_screen(4);

    put_pixel(x, SCREEN_HEIGHT / 2, 1);
    x += dir;
    if (x == 0 || x == SCREEN_WIDTH - 1) {
      dir = -dir;
    }

    swap_buffers();
  }
  return 0;
}
