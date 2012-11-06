#ifndef GPIO_H_
#define GPIO_H_ GPIO_H_

#include "inc/lm4f120h5qr.h"
#include "inc/hw_memmap.h"

#define GPIO_PIN_0	0x00000001  // pin 0
#define GPIO_PIN_1	0x00000002  // pin 1
#define GPIO_PIN_2	0x00000004  // pin 2
#define GPIO_PIN_3	0x00000008  // pin 3
#define GPIO_PIN_4	0x00000010  // pin 4
#define GPIO_PIN_5	0x00000020  // pin 5
#define GPIO_PIN_6	0x00000040  // pin 6
#define GPIO_PIN_7	0x00000080  // pin 7



/**
 * Enumeration of all available GPIO ports (and maybe some more).
 */
// Values are set to memory addresses directly to keep the code simple.
enum gpio_port {
	GPIO_A = GPIO_PORTA_AHB_BASE,
	GPIO_B = GPIO_PORTB_AHB_BASE,
	GPIO_C = GPIO_PORTC_AHB_BASE,
	GPIO_D = GPIO_PORTD_AHB_BASE,
	GPIO_E = GPIO_PORTE_AHB_BASE,
	GPIO_F = GPIO_PORTF_AHB_BASE,
	GPIO_G = GPIO_PORTG_AHB_BASE,
	GPIO_H = GPIO_PORTH_AHB_BASE,
	GPIO_J = GPIO_PORTJ_AHB_BASE,
};



static inline unsigned long GPIO_read(enum gpio_port port, unsigned char pins)
{
	return *((volatile unsigned long*) (port + (pins << 2)));
}

static inline void GPIO_write(enum gpio_port port, unsigned char pins, unsigned char value)
{
	*((volatile unsigned long*) (port + (pins << 2))) = value;
}



static inline void GPIO_set_output(enum gpio_port port, unsigned char pins, unsigned char output)
{
	if (output) {
		*((volatile unsigned long*) (port + 0x400)) |= pins;
	} else {
		*((volatile unsigned long*) (port + 0x400)) &= ~pins;
	}
}



// TODO: interrupt sense
// TODO: interrupt both edges
// TODO: interrupt interrupt event
// TODO: interrupt interrupt mask
// TODO: interrupt raw interrupt status
// TODO: interrupt masked interrupt status
// TODO: interrupt interrupt clear
// TODO: alternate function select



enum gpio_pin_drive {
	GPIO_DRIVE_2MA,
	GPIO_DRIVE_4MA,
	GPIO_DRIVE_8MA,
};

static inline void GPIO_set_drive_strength(enum gpio_port port, unsigned char pins, enum gpio_pin_drive drive)
{
	// base address of drive strength config registers is 0x500
	// three of them exist (one for 2mA, one for 4mA, one for 8mA) back to back
	switch (drive) {
		case GPIO_DRIVE_2MA:
			*((volatile unsigned long*) (port + 0x500)) = pins;
			break;

		case GPIO_DRIVE_4MA:
			*((volatile unsigned long*) (port + 0x504)) = pins;
			break;

		case GPIO_DRIVE_8MA:
			*((volatile unsigned long*) (port + 0x508)) = pins;
			break;
	}
}



// TODO: open drain select
// TODO: pull-up select
// TODO: pull-down select
// TODO: slew rate control select



static inline void GPIO_set_digital(enum gpio_port port, unsigned char pins, unsigned char enabled)
{
	if (enabled) {
		*((volatile unsigned long*) (port + 0x51c)) |= pins;
	} else {
		*((volatile unsigned long*) (port + 0x51c)) &= ~pins;
	}
}



// TODO: lock
// TODO: commit
// TODO: analog mode select
// TODO: port control
// TODO: adc control
// TODO: dma control
// TODO: peripheral id 4-7, 0-3
// TODO: prime cell id 0-3



static inline void GPIO_peripheral_enable(enum gpio_port port, unsigned char enabled)
{
	// port are all 0x1000 bytes apart, are consecutive, and start with port A
	// right now, i only know how to handle ports A through F, and i am too lazy to
	// search the datasheet for the other ports.
	if (port < GPIO_A || port > GPIO_F)
		return;

	if (enabled) {
		SYSCTL_GPIOHBCTL_R |= 1 << ((port - GPIO_A) / 0x1000);
	} else {
		SYSCTL_RCGC2_R &= ~(1 << ((port - GPIO_A) / 0x1000));
	}
}



#endif
