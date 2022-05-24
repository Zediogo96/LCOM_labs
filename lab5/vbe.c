#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <vbe.h>

int vbe_call(struct reg86 *r) {
  /* Make the BIOS call */
  return 0;
}

void *vbe_map_vram(unsigned int phys_base_ptr, unsigned int vram_size) {
  return NULL;
}

int vbe_set_mode(uint16_t mode) {
  return 0;
}
