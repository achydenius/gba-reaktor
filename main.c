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

  u32 vertex_count = 4;
  Vector3D vertices[] = {
      {0 << 8, 40 << 8}, {-40 << 8, 0 << 8}, {0 << 8, -40 << 8}, {40 << 8, 0 << 8}};
  Vector3D rotated[vertex_count];
  Vector2D projected[vertex_count];

  u32 angle = 0;
  Matrix matrix;
  while (1) {
    wait_vblank();

    clear_screen(4);

    create_rotation_matrix(&matrix, 0, angle >> 1, angle);

    for (u32 i = 0; i < vertex_count; i++) {
      transform_vector(&vertices[i], &matrix, &rotated[i]);
      project_vector(&rotated[i], 80 << 8, 60 << 8, &projected[i]);
    }

    draw_polygon(projected, vertex_count, 1);

    angle = (angle + 1) & (TRIG_TABLE_SIZE - 1);

    swap_buffers();
  }
  return 0;
}
