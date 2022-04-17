#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define KBD_STAT_REG 0x64
#define KBD_OUT_BUF 0X60

#define KBD_IRQ 1

#define KBC_OUTPUT_BUFFER_FULL BIT(0)

#define KBC_PARITY_ERROR BIT(7)
#define KBC_TIMEOUT_ERROR BIT(6)

#define ESC_BREAKCODE 0x81


#define DELAY_US 20000

#endif // _LCOM_I8042_H_

