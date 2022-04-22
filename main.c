#include <gba.h>
#include <gba_video.h>
#include <maxmod.h>

#include "math.h"
#include "objects.h"
#include "renderer.h"
#include "soundbank.h"
#include "soundbank_bin.h"

typedef struct Star {
  u32 x, y;
} Star;

Star stars[][10] = {{{216, 98},
                     {194, 107},
                     {10, 66},
                     {130, 124},
                     {103, 77},
                     {122, 91},
                     {149, 55},
                     {129, 35},
                     {72, 35},
                     {193, 24}},
                    {{158, 64},
                     {232, 136},
                     {180, 154},
                     {230, 37},
                     {79, 25},
                     {186, 18},
                     {230, 84},
                     {120, 143},
                     {25, 90},
                     {111, 80}},
                    {{156, 52},
                     {141, 122},
                     {113, 133},
                     {66, 15},
                     {206, 140},
                     {234, 3},
                     {23, 102},
                     {181, 0},
                     {156, 126},
                     {211, 85}}};

void wait_vblank() {
  while (REG_VCOUNT < SCREEN_HEIGHT) {
  }
}

void swap_buffers() {
  REG_DISPCNT ^= BACKBUFFER;
  g_buffer = REG_DISPCNT & BACKBUFFER ? (u16*)MODE5_FB : (u16*)MODE5_BB;
}

void vblank_handler() { mmFrame(); }

int main() {
  for (u32 i = 0; i < 256; i++) {
    BG_PALETTE[i] = palette[i];
  }

  REG_DISPCNT = MODE_4 | BG2_ON;

  irqInit();
  irqSet(IRQ_VBLANK, mmVBlank);
  irqEnable(IRQ_VBLANK);
  mmSetVBlankHandler(&vblank_handler);

  mmInitDefault(soundbank_bin, 8);
  mmStart(MOD_LOOP_DRONE, MM_PLAY_LOOP);

  u32 tick = 0;
  while (1) {
    clear_screen(4);

    u32 trig_table_wrap = TRIG_TABLE_SIZE - 1;

    vector_set(&object.rotation, (tick << 1) & trig_table_wrap, (tick << 3) & trig_table_wrap, 0);
    vector_set(&object.translation, 0, 0, -220 << 8);

    for (u32 i = 0; i < 3; i++) {
      for (u32 j = 0; j < 10; j++) {
        put_pixel((stars[i][j].x + (tick << i)) & 0xFF, stars[i][j].y & 0xFF, 0x3F << i);
      }
    }

    object_render(&object);

    tick++;

    wait_vblank();
    swap_buffers();
  }
  return 0;
}
