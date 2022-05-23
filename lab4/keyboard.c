#include <lcom/lcf.h>
#include <minix/sysutil.h>
#

#include "keyboard.h"


int out_byte;

int kbd_read_outb() {


}

void (kbd_ih)() {

    while(1) {
        
        if (kbd_read_outb() == 0) break;
        tickdelay(micros_to_ticks(DELAY_US));
    }
}

void (kbd_poll)() {
    kbd_read_outb();
    tickdelay(micros_to_ticks(DELAY_US));
}

static int hook_id;

int (kbd_subscribe_int)(uint8_t *bit_no) {

    hook_id = KBD_IRQ;
    *bit_no = hook_id;

    if (sys_irqsetpolicy(KBD_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id)) return 1;

    return 0;

}

int(kbd_unsubscribe_int)() {

    if (sys_irqrmpolicy(&hook_id)) return 1;

    return 0;
}

int (kbc_issue_command)(uint8_t cmd, uint8_t port) {

}
