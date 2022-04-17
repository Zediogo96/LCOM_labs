#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "utils.h"
#include <minix/sysutil.h>

#include "i8042.h"


void (kbd_ih)();

int kbd_read_outb();

int (kbd_subscribe_int)(uint8_t *bit_no);

int (kbd_unsubscribe_int)();


#endif // _KEYBOARD_H_
