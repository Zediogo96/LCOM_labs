#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include <minix/syslib.h>

#include "i8254.h"

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  if (timer < 0 || timer > 2 || st == NULL) return 1;
  
  uint8_t RB_CMD = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_;

  if (sys_outb(0x43, RB_CMD)) return 1;

  if (util_sys_inb(TIMER_0 + timer, st)) return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  
  union timer_status_field_val conf;

  uint8_t in_mode;

  switch (field) {

    case tsf_all:
      conf.byte = st;
      break;

    case tsf_initial:

      in_mode = (st & (BIT(5) | BIT(4))) >> 4; 

      switch(in_mode) {

        case 0: conf.in_mode = INVAL_val; break;
        case 1: conf.in_mode = LSB_only; break;
        case 2: conf.in_mode = MSB_only; break;
        case 3: conf.in_mode = MSB_after_LSB; break;
        default: return 1;
      }
      break;

    case tsf_mode:
      conf.count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;

      if (conf.count_mode == 6) conf.count_mode = 2;
      if (conf.count_mode == 7) conf.count_mode = 3;

      break;
    
    case tsf_base: 

      conf.count_mode = (st & BIT(0));
      break;

    default: return 1;
  }

  return timer_print_config(timer, field, conf);
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  

  uint16_t IV = TIMER_FREQ / freq;

  uint8_t CMD_WORD = TIMER_LSB_MSB, timer_port, status, iv_lsb, iv_msb;

  switch(timer) {
    
    case 0: 
      CMD_WORD |= TIMER_SEL0;
      timer_port = TIMER_0;
      break;

    case 1:
      CMD_WORD |= TIMER_SEL1;
      timer_port = TIMER_1;
      break;
    
    case 2:
      CMD_WORD |= TIMER_SEL2;
      timer_port = TIMER_2;
      break;

    default: return 1;
  }

  if (timer_get_conf(timer, &status)) return 1;
 
  /** 4 LSB should not be changed, according to PP **/
  uint8_t mask = 0x0F;
  CMD_WORD |= (status & mask);

  if (sys_outb(TIMER_CTRL, CMD_WORD)) return 1;

  util_get_LSB(IV, &iv_lsb);
  util_get_MSB(IV, &iv_msb);

  if (sys_outb(timer_port, iv_lsb) || sys_outb(timer_port, iv_msb)) return 1;

  return 0;
}

static int hook_id;
int (timer_subscribe_int)(uint8_t *bit_no) {
  
  hook_id = TIMER0_IRQ;
  *bit_no = hook_id;

  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)) return 1;
  
  return 0;
}


int (timer_unsubscribe_int)() {
  
  return sys_irqrmpolicy(&hook_id);
}

int no_interrupts = 0;

void (timer_int_handler)() {
  no_interrupts++;
}




