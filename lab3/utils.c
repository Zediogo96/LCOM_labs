#include <lcom/lcf.h>

#include <stdint.h>

#include <minix/syslib.h>



int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  
  *lsb = (uint8_t) val;

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  
  *msb = (uint8_t)(val >> 8);

  return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {

    uint32_t tmp;
    if (sys_inb(port, &tmp)) return 1;

    *value = tmp;

    return 0;
}


bool isMakeCode(uint8_t code) {

    if (code & BIT(7)) return false;
    return true;
}
