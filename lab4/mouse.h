#ifndef _MOUSE_H_
#define _MOUSE_H_

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

int (kbc_issue_command)(uint8_t cmd, int port);

#endif
