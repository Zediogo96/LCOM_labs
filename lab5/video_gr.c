#include "video_gr.h"

static uint16_t h_res;
static uint16_t v_res;
static unsigned bits_per_pixel;
static unsigned bytes_per_pixel;

static unsigned vram_base;
static unsigned vram_size;

static vbe_mode_info_t inf;

static void *video_mem;

/**
 * @file video_gr.c
 * @author your name (you@domain.com)
 * @brief Initializes the video module in graphics mode.
        Uses the VBE INT 0x10 interface to set the desired graphics mode using a linear frame buffer, maps VRAM to the process' address space and initializes static global variables
        with the resolution of the screen, and the color depth (i.e the no. of bits per pixel).
        Initially, you should call function vbe_get_mode_info() provided by the LCF, to get this information. Later, you can implement your own version of this function.
 * @version 0.1
 * @date 2022-05-16
 *
 * @copyright Copyright (c) 2022
 *
 */

void *(vg_init) (uint16_t mode) {
  if (vbe_get_mode_info(mode, &inf) != 0) { // if Operation fail
    printf("vg_init(): failed to get mode \n");
    return NULL;
  }

  struct reg86 r86;
  int r;
  struct minix_mem_range mr;

  h_res = inf.XResolution;
  v_res = inf.YResolution;
  bits_per_pixel = inf.BitsPerPixel;
  bytes_per_pixel = (bits_per_pixel + 7) / 8;

  vram_base = inf.PhysBasePtr;
  vram_size = h_res * v_res * bytes_per_pixel;

  /* Allow memory mapping */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return NULL;
  }
  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED) {
    panic("couldn't map video memory");
    return NULL;
  }

  memset(&r86, 0, sizeof(r86));

  r86.ax = 0x4F02;
  r86.bx = BIT(14) | mode;
  r86.intno = 0x10;

  if (sys_int86(&r86) != 0) { // if Operation fail
    printf("vg_init(): sys_int86() failed \n");
    return NULL;
  }

  return video_mem;
}

int(change_pixel_color)(uint16_t x, uint16_t y, uint32_t color) {
  uint8_t *mem = video_mem;
  mem += (x * bytes_per_pixel) + (y * h_res * bytes_per_pixel);
  memcpy(mem, &color, bytes_per_pixel);
  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = 0; i < len; i++) {
    change_pixel_color(x + i, y, color);
  }
  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {


  for (int i = 0; i < height; i++) {
    vg_draw_hline(x, y + i, width, color);
  }

  return 0;
}

void vg_draw_image(xpm_image_t img, uint8_t *map, uint16_t x, uint16_t y) {
}
