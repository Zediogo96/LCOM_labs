#include <mouse.h>

#include "i8042.h"

static int hookID;
uint8_t out_byte;

int mouse_subscribe_int(uint8_t *bit_no) {

  hookID = 12;
  *bit_no = hookID;
  if (sys_irqsetpolicy(hookID, IRQ_EXCLUSIVE | IRQ_REENABLE, &hookID))
    return 1;

  return 0;
}

int mouse_unsubscribe_int() {

  if (sys_irqrmpolicy(&hookID))
    return 1;
  return 0;
}

void(mouse_ih)() {

  while (true) {
    if (kbc_read_outb() == 0) {
      break;
    }
    else {
      tickdelay(micros_to_ticks(20000));
    }
  }
}

/**
 * @brief This function is used to process the packets, see slide 32 of 06-Mouse.pdf for more information
 *
 * @param bytes
 * @return struct packet
 */

// struct packet {
//      uint8_t bytes[3];
//      bool rb;
//      bool mb;
//      bool lb;
//      int16_t delta_x;
//      int16_t delta_y;
//      bool x_ov;
//      bool y_ov;
//  };
struct packet process_packets(uint8_t *bytes) {

  struct packet pp;

  bool msb_x, msb_y;

  for (int i = 0; i < 3; i++) pp.bytes[i] = bytes[i];

  pp.lb = bytes[0] & BIT(0);
  pp.rb = bytes[0] & BIT(1);
  pp.mb = bytes[0] & BIT(2);

  msb_x = bytes[0] & BIT(4);
  msb_y = bytes[0] & BIT(5);

  pp.x_ov = bytes[0] & BIT(6);
  pp.y_ov = bytes[0] & BIT(7);

  pp.delta_x = bytes[1];
  pp.delta_y = bytes[2];

  for (int i = 8; i < 16; i++) {
    pp.delta_x += (msb_x << i);
  }

  for (int i = 8; i < 16; i++) {
    pp.delta_y += (msb_y << i);
  }

  return pp;
}

int mouse_disable_reporting() {
  uint8_t status = 0;
  if (kbc_issue_command(0xD4, 0x64))
    return 1;
  if (kbc_issue_command(0xF5, 0x60))
    return 1;

  if (util_sys_inb(0x60, &status))
    return 1;
  if (status == 0xFA)
    return 0;
  return 1;
}
int kbc_read_outb() {
  uint8_t status, data;
  if (util_sys_inb(0x64, &status))
    return 1;

  if (status & BIT(0)) { // output buffer full
    if (util_sys_inb(0x60, &data))
      return 1;
    if (status & (BIT(7) | BIT(6))) {
      printf("parity or timeout error ocurred");
    }
    else {
      out_byte = data;
      return 0;
    }
  }

  return 1;
}

int kbc_issue_command(uint32_t cmd, int port) {
  uint8_t status = 0;
  if (util_sys_inb(0x64, &status)) {
    if ((status & BIT(1)) == 0) {
      printf("input buffer full");
      return 1;
    }
    else {
      sys_outb(0x60, cmd);
      return 0;
    }
  }

  return 1;
}
