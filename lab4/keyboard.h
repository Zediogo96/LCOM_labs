#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "utils.h"
#include <minix/sysutil.h>

#include "i8042.h"

#include "i8254.h"


void (kbd_ih)();

void (kbd_poll)();

int kbd_read_outb();

int (kbd_subscribe_int)(uint8_t *bit_no);

int (kbd_unsubscribe_int)();

int (kbc_issue_command)(uint8_t cmd, uint8_t port);


#endif // _KEYBOARD_H_
