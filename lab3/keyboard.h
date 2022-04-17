#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "i8042.h"
#include "i8254.h"
#include "utils.h"

int kbc_read_outb();

int kbd_subscribe_int(uint8_t *bit_no);

int kbd_unsubscribe_int();

int kbc_issue_command(uint8_t cmd, int port);

void (kbc_poll)();

void(kbc_ih)();

#endif //_KEYBOARD_H_
