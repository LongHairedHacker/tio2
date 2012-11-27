#include "tio2/gpio.hpp"
#include "tio2/clock.hpp"

//*****************************************************************************
//
// Define pin to LED color mapping.
//
//*****************************************************************************

#define RED_LED   2
#define BLUE_LED  4
#define GREEN_LED 8

using namespace tio2;

int main(void)
{
	clock.config(
			clock.config()
				.pll_powerdown(false)
				.pll_bypass(false)
				.sysdiv(25)
				.xtal(XTAL::X_16_MHz)
				.main_oscillator_disable(false)
				.oscillator_source(OscSource::Main)
	);

	//
	// Enable and configure the GPIO port for the LED operation.
	//
	gpio_in_run.enable(GPIO::Port::A, GPIO::Port::E, GPIO::Port::F);
	while (!gpio_status.ready(GPIO::Port::F));

	gpio_f.outputs() |= RED_LED | BLUE_LED | GREEN_LED;
	gpio_f.digital_enable() |= RED_LED | BLUE_LED | GREEN_LED;

	//
	// Loop Forever
	//
	while(1)
	{
		//
		// Turn on the LED
		//
		//GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, RED_LED);
		gpio_f[RED_LED | BLUE_LED] = 0;

		for (int i = 0; i < 1600000; i++) { __asm__ __volatile__ ("nop"); }

		//
		// Delay for a bit
		//
		//SysCtlDelay(2000000);

		//
		// Turn on the LED
		//
		//GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, BLUE_LED);
		gpio_f[RED_LED | BLUE_LED | GREEN_LED] = RED_LED | BLUE_LED;

		for (int i = 0; i < 1600000; i++) { __asm__ __volatile__ ("nop"); }

		//
		// Delay for a bit
		//
		//SysCtlDelay(2000000);
	}

	return 0;
}
