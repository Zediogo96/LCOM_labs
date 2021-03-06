#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>
#include <stdio.h>

#pragma once

int(util_get_LSB)(uint16_t val, uint8_t *lsb);

int(util_get_MSB)(uint16_t val, uint8_t *msb);

int(util_sys_inb)(int port, uint8_t *value);


#endif
