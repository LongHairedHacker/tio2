#ifndef GPIO_H_
#define GPIO_H_ GPIO_H_

#include "inc/lm4f120h5qr.h"
#include "inc/hw_memmap.h"

#define GPIO_EN_J   SYSCTL_RCGC2_GPIOJ
#define GPIO_EN_H   SYSCTL_RCGC2_GPIOH
#define GPIO_EN_G   SYSCTL_RCGC2_GPIOG
#define GPIO_EN_F   SYSCTL_RCGC2_GPIOF
#define GPIO_EN_E   SYSCTL_RCGC2_GPIOE
#define GPIO_EN_D   SYSCTL_RCGC2_GPIOD
#define GPIO_EN_C   SYSCTL_RCGC2_GPIOC
#define GPIO_EN_B   SYSCTL_RCGC2_GPIOB
#define GPIO_EN_A   SYSCTL_RCGC2_GPIOA

#define GPIO_PIN_0	0x00000001  // pin 0
#define GPIO_PIN_1	0x00000002  // pin 1
#define GPIO_PIN_2	0x00000004  // pin 2
#define GPIO_PIN_3	0x00000008  // pin 3
#define GPIO_PIN_4	0x00000010  // pin 4
#define GPIO_PIN_5	0x00000020  // pin 5
#define GPIO_PIN_6	0x00000040  // pin 6
#define GPIO_PIN_7	0x00000080  // pin 7

#define GPIO_OF_DATA	0x000
#define GPIO_OF_DIR 	0x400
#define GPIO_OF_DEN 	0x51c




static inline void GPIO_enable(unsigned long gpio) {
	SYSCTL_RCGC2_R |= gpio;
} 

static inline void GPIO_disable(unsigned long gpio) {
	SYSCTL_RCGC2_R &= ~gpio;
}


#define DECLARE_BIT_SETTER(name,offset) static inline \
	void name(unsigned long basereg, unsigned long pins) \
	{ \
		*((volatile unsigned long *) (basereg + offset)) |= pins; \
	} 

#define DECLARE_BIT_CLEARER(name,offset) static inline \
	void name(unsigned long basereg, unsigned long pins) \
	{ \
		*((volatile unsigned long *) (basereg + offset)) &= ~pins; \
	}


DECLARE_BIT_SETTER(GPIO_set_output, GPIO_OF_DIR)
DECLARE_BIT_CLEARER(GPIO_set_input, GPIO_OF_DIR)

DECLARE_BIT_SETTER(GPIO_set_digital, GPIO_OF_DEN)
DECLARE_BIT_CLEARER(GPIO_unset_digital, GPIO_OF_DEN)



/* 
 * Since the arm assembler does not have a set or clear bit instruction, 
 * it can't set or cler bit in one cycle.
 * But It can write data to a memory location in one cycle.
 * So Setting GPIO pins is done by writing a byte to register,
 * which adress is made up of a base address plus a offset.
 * 
 * 
 *
 */

static inline void GPIO_set_pin(unsigned long basereg, unsigned long pins)
	{ 
		*((volatile unsigned long *) (basereg + GPIO_OF_DATA + pins*4)) = 0xFF; 
	}


static inline void GPIO_clear_pin(unsigned long basereg, unsigned long pins)
	{ 
		*((volatile unsigned long *) (basereg + GPIO_OF_DATA + pins*4)) = 0x00; 
	}






#endif