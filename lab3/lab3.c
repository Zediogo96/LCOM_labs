#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"
#include "timer.c"


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

extern uint8_t out_byte;


int (kbd_test_scan)() {

  uint8_t bit_no = 1; // IRQ DO KBD 
  uint32_t irq_set = BIT(bit_no);
 
  int ipc_status, r, size = 0;
  
  uint8_t bytes[2];
  message msg;

  kbd_subscribe_int(&bit_no);
  
  while(bytes[0] != ESC_BREAKCODE) { /* You may want to use a different condition */

      /* Get a request message. */
      if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
      }

      if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                    
                    kbc_ih();
                    bytes[size] = out_byte;

                    if (out_byte == 0) return 1;
                    

                    if (out_byte != 0xE0) {
                      kbd_print_scancode(isMakeCode(out_byte), size + 1, bytes);
                      size = 0;
                    }
                    else size++;
                }
                
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
      } else { /* received a standard message, not a notification */
          /* no standard messages expected: do nothing */
      }
    }


  kbd_unsubscribe_int();

  return 0;
}

int (kbd_test_poll)() {

  uint8_t bytes[2];
  int size = 0;

  while (out_byte != ESC_BREAKCODE) {
    kbc_poll();

    if (out_byte) { // means poll got smth
      bytes[size] = out_byte;

      if (out_byte == 0xE0) size++;
      else {
        kbd_print_scancode(isMakeCode(out_byte), size + 1, bytes);
        size = 0;
      }
    }
  }

  kbc_issue_command(0x20, 0x64);
  kbc_read_outb();

  kbc_issue_command(0x60, 0x64);
  kbc_issue_command((out_byte | BIT(0)), 0x60);
  
  return 0;
}

extern int no_interrupts;

int (kbd_test_timed_scan)(uint8_t n) {
  
  uint8_t bytes[2];

  int ipc_status, r, size = 0;
  message msg;

  uint8_t time = n;

  no_interrupts = 0;

  /** TIMER INTERRUPT HANDLING **/ 

  uint8_t timer_id = TIMER0_IRQ;
  uint32_t timer_irq_set = BIT(timer_id);

  if (timer_subscribe_int(&timer_id)) return 1;

  /** KEYBOARD INTERRUPT HANDLING **/ 
  uint8_t kbd_id = KBC_IRQ;
  uint32_t kbd_irq_set = BIT(kbd_id);

  if (kbd_subscribe_int(&kbd_id)) return 1;


  while (bytes[0] != 0x81 && time) {

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) /** received a notifiation **/
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /** hardware interrupt notification **/

          if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();
            if (no_interrupts % 60 == 0) time--;
          }

          if (msg.m_notify.interrupts & kbd_irq_set) {
            /** RESET IT **/
            time = n; no_interrupts = 0;

            kbc_ih();

            bytes[size] = out_byte; // we get out_byte from kbc_ih

            if (out_byte == 0) return 1;

            if (out_byte != 0xE0) {
              kbd_print_scancode(isMakeCode(out_byte), size + 1, bytes);
              size = 0;
            }
            else size++;
          }
          break;
          default:
          break;
      }
      else {

      }
  }

  timer_unsubscribe_int();
  kbd_unsubscribe_int();
  
  return 0;
}
