#include "gpio.hpp"

static void _gpio_init_ahb(void)
{
	// put gpio pins onto the AHB bus
	*((volatile uint32_t*) 0x400FE06c) |= 0x3f;
}

__attribute__ ((section(".startup_hook")))
extern void (*const _gpio_init)(void) = &_gpio_init_ahb;
