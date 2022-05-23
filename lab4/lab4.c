// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"

#include "mouse.h"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

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

int(mouse_test_packet)(uint32_t cnt) {

  uint8_t bit_no = 12;
  uint32_t mouse_irq = BIT(12);
  mouse_enable_data_reporting();

  if (mouse_subscribe_int(&bit_no)) {
    printf("%s: mouse_subscribe_int() failed\n", __func__);
    return 1;
  }

  int r, ipc_status;
  message msg;

  // SIMILAR TO KEYBOARD, BUT HAS ONE ADDITIONAL BYTE
  uint8_t bytes[3];
  uint8_t byte_count = 0;
  uint32_t packets_received = 0;

  while (packets_received < cnt) { /* You may want to use a different condition */
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & mouse_irq) {
            mouse_ih();

            bytes[byte_count] = out_byte;
            byte_count++;

            if (byte_count == 3) {
              byte_count = 0;
              packets_received++;

              struct packet pp = process_packets(bytes);

              mouse_print_packet(&pp);
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (mouse_unsubscribe_int() != 0) {
    printf("%s: mouse_unsubscribe_int() failed\n", __func__);
  }
  if (mouse_disable_reporting() != 0) {
    printf("%s: mouse_disable_reporting() failed\n", __func__);
  }
  return 0;
}

int(mouse_test_async)(uint8_t idle_time) {
  /* To be completed */
  printf("%s(%u): under construction\n", __func__, idle_time);
  return 0;
}
