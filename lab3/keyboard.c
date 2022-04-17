#include <lcom/lcf.h>
#include <minix/sysutil.h>

#include <stdint.h>

#include "keyboard.h"

int hook_id;

uint8_t out_byte;

int kbc_read_outb() {
    
    out_byte = 0;
    uint8_t status;

    // MEANING THAT WE WANT TO PERFORM A READ
    if (util_sys_inb(KBC_ST_REG, &status) != 0) return 1; // REG 0x64


    if (status & KBC_OBF_FLAG) {
        uint8_t data;

        /** We do this first step because it is said that the register should be read regardless **/
        if (util_sys_inb(KBC_OUTPUT_BUF_REG, &data)) return 1;

        if ((status & (KBC_PARITY_ERROR | KBC_TIMEOUT_ERROR)) == 0) {
            out_byte = data;
            return 0;
        }

        else {
            printf("%s: Parity or Timeout error ocurred \n", __func__); 
            return 1;
        }
    }

    return 1;
}

void (kbc_ih)() {
    while (1) {
        if (kbc_read_outb() == 0) {
            break;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
    
}

void (kbc_poll)(){
    kbc_read_outb();
    tickdelay(micros_to_ticks(DELAY_US));
}

int kbc_issue_command(uint8_t cmd, int port) {

    uint8_t stat = 0;

    int cycles = 0;

    while (cycles++ < 20) {

        if (util_sys_inb(KBC_ST_REG , &stat) != 0) return 1;

        if ((stat & KBC_IBF_FLAG) == 0) {
            sys_outb(port, cmd);
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
    return 1;
}


int kbd_subscribe_int(uint8_t *bit_no) {
    hook_id = KBC_IRQ; // 1

    *bit_no = hook_id;

    if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)) return 1;

    return 0;
}

int kbd_unsubscribe_int() {
    
    if (sys_irqrmpolicy(&hook_id)) return 1;

    return 0;
}
