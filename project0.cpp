#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

#include "tio2/gpio.hpp"

//*****************************************************************************
//
// Define pin to LED color mapping.
//
//*****************************************************************************

#define RED_LED   2
#define BLUE_LED  4
#define GREEN_LED 8


int
main(void)
{
    //
    // Setup the system clock to run at 50 Mhz from PLL with crystal reference
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                    SYSCTL_OSC_MAIN);

    //
    // Enable and configure the GPIO port for the LED operation.
    //
	gpio_enable(gpio_f);

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
		gpio_f[RED_LED | BLUE_LED] = 0xff;

        //
        // Delay for a bit
        //
        //SysCtlDelay(2000000);

        //
        // Turn on the LED
        //
        //GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, BLUE_LED);
		gpio_f[RED_LED | BLUE_LED] = BLUE_LED;

        //
        // Delay for a bit
        //
        //SysCtlDelay(2000000);
    }
}
