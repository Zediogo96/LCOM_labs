#pragma once


#include <stdint.h>


int(util_get_LSB)(uint16_t val, uint8_t *lsb);

int(util_get_MSB)(uint16_t val, uint8_t *msb);

int (util_sys_inb)(int port, uint8_t *value);

bool (isMakeCode)(uint8_t scan_code);
