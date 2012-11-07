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

#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3


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
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIO_peripheral_enable(GPIO_F, true);

    //GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED);

//    GPIO_set_output(GPIO_F, RED_LED | BLUE_LED | GREEN_LED, true);
	gpio_f.outputs() |= RED_LED | BLUE_LED | GREEN_LED;
    GPIO_set_digital(GPIO_F, RED_LED | BLUE_LED | GREEN_LED, true);

    //GPIO_PORTF_DIR_R = 0x08;
    //GPIO_PORTF_DEN_R = 0x08;
    //
    // Loop Forever
    //
    while(1)
    {
        //
        // Turn on the LED
        //
        //GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, RED_LED);
//        GPIO_write(GPIO_F, RED_LED | BLUE_LED, RED_LED | BLUE_LED);
		gpio_f[RED_LED | BLUE_LED] = 0xff;

        //
        // Delay for a bit
        //
        //SysCtlDelay(2000000);

        //
        // Turn on the LED
        //
        //GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, BLUE_LED);
//        GPIO_write(GPIO_F, RED_LED | BLUE_LED, BLUE_LED);
		gpio_f[RED_LED | BLUE_LED] = BLUE_LED;

        //
        // Delay for a bit
        //
        //SysCtlDelay(2000000);
    }
}