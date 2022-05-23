#include <mouse.h>

#include "i8042.h"


int mouse_subscribe_int(uint8_t *bit_no) {

}

int mouse_unsubscribe_int() {
 
}

void(mouse_ih)() {

}

/**
 * @brief This function is used to process the packets, see slide 32 of 06-Mouse.pdf for more information
 *
 * @param bytes
 * @return struct packet
 */
struct packet process_packets(uint8_t *bytes) {

}

int mouse_disable_reporting() {

}

int kbc_read_outb(){
 
}


int kbc_issue_command(uint8_t cmd, int port) {



}

