#include <lcom/lcf.h>
#include <minix/sysutil.h>
#

#include "keyboard.h"


int out_byte;

int kbd_read_outb() {

    uint8_t status;
    out_byte = 0;

    if (util_sys_inb(KBD_STAT_REG, &status)) return 1;  

    if (status & KBC_OUTPUT_BUFFER_FULL) {

        uint8_t tmp_data;

        if (util_sys_inb(KBD_OUT_BUF, &tmp_data)) return 1;

        if ((status & (KBC_PARITY_ERROR | KBC_TIMEOUT_ERROR)) == 0) {
            out_byte = tmp_data;
            return 0;
        }
        else {
            printf("Parity or Timeout error!");
            return 1;
        }
    }
    return 1;
}

void (kbd_ih)() {

    while(1) {
        
        if (kbd_read_outb() == 0) break;
        tickdelay(micros_to_ticks(DELAY_US));
    }
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
