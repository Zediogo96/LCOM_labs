#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_


/** Errors **/
#define KBC_PARITY_ERROR BIT(7)
#define KBC_TIMEOUT_ERROR BIT(6)
#define ESC_BREAKCODE 0x81

#define KBC_ST_REG 0x64
#define KBC_OUTPUT_BUF_REG 0x60


#define KBC_IRQ 1
#define DELAY_US 20000



#define KBC_IBF_FLAG BIT(1)
#define KBC_OBF_FLAG BIT(0)





#endif // _LCOM_I8042_H_
