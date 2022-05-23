#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <lcom/lcf.h>
#include "utils.h"
#include "i8042.h"

#include <stdint.h>
#include <stdio.h>

int kbc_read_outb();

void (mouse_ih)();

int mouse_subscribe_int(uint8_t * bit_no);
int mouse_unsubscribe_int();

struct packet process_packets(uint8_t * bytes);

int mouse_disable_reporting();

int kbc_issue_command(uint32_t cmd, int port);

#define MOUSE_LB_CLICK BIT(0)
#define MOUSE_RB_CLICK BIT(1)
#define MOUSE_MB_CLICK BIT(2)

#define MOUSE_X_SIGN BIT(4)
#define MOUSE_Y_SIGN BIT(5)

#define MOUSE_X_OVFL BIT(6)
#define MOUSE_Y_OVFL BIT(7)


#endif
