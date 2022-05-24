#include "video_gr.h"

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
  return NULL;
}

int(change_pixel_color)(uint16_t x, uint16_t y, uint32_t color) {

  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  return 0;
}

void vg_draw_image(xpm_image_t img, uint8_t *map, uint16_t x, uint16_t y) {
}
