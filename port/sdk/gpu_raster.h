/* The software GPU (gpu_raster.c), used by libgpu's PC version (gpu.c). */
#ifndef PC_GPU_RASTER_H
#define PC_GPU_RASTER_H

#include <stdint.h>

#define VRAM_W 1024
#define VRAM_H 512

extern uint16_t Vram[VRAM_H][VRAM_W];

void gpu_run_packet(const uint32_t *p, int len);
void gpu_fill(int x, int y, int w, int h, uint32_t rgb);
void gpu_load_image(int x, int y, int w, int h, const uint16_t *p);
void gpu_store_image(int x, int y, int w, int h, uint16_t *p);
void gpu_move_image(int sx, int sy, int dx, int dy, int w, int h);
void gpu_set_draw_env(int x0, int y0, int x1, int y1, int ofx, int ofy, uint32_t tpage_word, uint32_t win);

#endif
