#include "inc/hw_nvic.h"
#include "inc/hw_types.h"


#define DECLARE_ISR(name) void name(void) __attribute__((weak, alias("BadISR")));

void BadISR(void) __attribute__((weak));

void ResetISR(void) __attribute__((weak));
DECLARE_ISR(NmiSR)
DECLARE_ISR(FaultISR)
DECLARE_ISR(MPUFaultISR)
DECLARE_ISR(BusFaultISR)
DECLARE_ISR(UsageFaultISR)
DECLARE_ISR(SVCallISR)
DECLARE_ISR(DebugMonitorISR)
DECLARE_ISR(PendSVISR)
DECLARE_ISR(SysTickISR)
DECLARE_ISR(GPIOPortAISR)
DECLARE_ISR(GPIOPortBISR)
DECLARE_ISR(GPIOPortCISR)
DECLARE_ISR(GPIOPortDISR)
DECLARE_ISR(GPIOPortEISR)
DECLARE_ISR(UART0RxTxISR)
DECLARE_ISR(UART1RxTxISR)
DECLARE_ISR(SSIORxTxISR)
DECLARE_ISR(I2C0MasterSlaveISR)
DECLARE_ISR(PWMFaultISR)
DECLARE_ISR(PWM0Generator0ISR)
DECLARE_ISR(PWM0Generator1ISR)
DECLARE_ISR(PWM0Generator2ISR)
DECLARE_ISR(QuadratureEncoder0ISR)
DECLARE_ISR(ADC0Sequence0ISR)
DECLARE_ISR(ADC0Sequnece1ISR)
DECLARE_ISR(ADC0Sequnece2ISR)
DECLARE_ISR(ADC0Sequnece3ISR)
DECLARE_ISR(WatchdogTimerISR)
DECLARE_ISR(Timer0AISR)
DECLARE_ISR(Timer0BISR)
DECLARE_ISR(Timer1AISR)
DECLARE_ISR(Timer1BISR)
DECLARE_ISR(Timer2AISR)
DECLARE_ISR(Timer2BISR)
DECLARE_ISR(AnalogComparator0ISR)
DECLARE_ISR(AnalogComparator1ISR)
DECLARE_ISR(AnalogComparator2ISR)
DECLARE_ISR(SystemControlISR)
DECLARE_ISR(FlashControlISR)
DECLARE_ISR(GPIOPortFISR)
DECLARE_ISR(GPIOPortGISR)
DECLARE_ISR(GPIOPortHISR)
DECLARE_ISR(UART2RxTxISR)
DECLARE_ISR(SSI1RxTxISR)
DECLARE_ISR(Timer3AISR)
DECLARE_ISR(Timer3BISR)
DECLARE_ISR(I2C1MasterSlaveISR)
DECLARE_ISR(QuadratureEncoder1ISR)
DECLARE_ISR(CAN0ISR)
DECLARE_ISR(CAN1ISR)
DECLARE_ISR(CAN2ISR)
DECLARE_ISR(EthernetISR)
DECLARE_ISR(HibernateISR)
DECLARE_ISR(USB0ISR)
DECLARE_ISR(PWMGenerator3ISR)
DECLARE_ISR(uDMASoftwareTransferISR)
DECLARE_ISR(uDMAErrorISR)
DECLARE_ISR(ADC1Sequnece0ISR)
DECLARE_ISR(ADC1Sequnece1ISR)
DECLARE_ISR(ADC1Sequnece2ISR)
DECLARE_ISR(ADC1Sequnece3ISR)
DECLARE_ISR(I2S0ISR)
DECLARE_ISR(ExternalBusISR)
DECLARE_ISR(GPIOPortJISR)
DECLARE_ISR(GPIOPortKISR)
DECLARE_ISR(GPIOPortLISR)
DECLARE_ISR(SSI2RxTxISR)
DECLARE_ISR(SSI3RxTxISR)
DECLARE_ISR(UART3RxTxISR)
DECLARE_ISR(UART4RxTxISR)
DECLARE_ISR(UART5RxTxISR)
DECLARE_ISR(UART6RxTxISR)
DECLARE_ISR(UART7RxTxISR)
DECLARE_ISR(I2C2MasterSlaveISR)
DECLARE_ISR(I2C3MasterSlaveISR)
DECLARE_ISR(Timer4AISR)
DECLARE_ISR(Timer4BISR)
DECLARE_ISR(Timer5AISR)
DECLARE_ISR(Timer5BISR)
DECLARE_ISR(WideTimer0AISR)
DECLARE_ISR(WideTimer0BISR)
DECLARE_ISR(WideTimer1AISR)
DECLARE_ISR(WideTimer1BISR)
DECLARE_ISR(WideTimer2AISR)
DECLARE_ISR(WideTimer2BISR)
DECLARE_ISR(WideTimer3AISR)
DECLARE_ISR(WideTimer3BISR)
DECLARE_ISR(WideTimer4AISR)
DECLARE_ISR(WideTimer4BISR)
DECLARE_ISR(WideTimer5AISR)
DECLARE_ISR(WideTimer5BISR)
DECLARE_ISR(FPUISR)
DECLARE_ISR(PECI0ISR)
DECLARE_ISR(LPC0ISR)
DECLARE_ISR(I2C4MasterSlaveISR)
DECLARE_ISR(I2C5MasterSlaveISR)
DECLARE_ISR(CPIOPortMISR)
DECLARE_ISR(CPIOPortNISR)
DECLARE_ISR(QuadratureEncoder2ISR)
DECLARE_ISR(Fan0ISR)
DECLARE_ISR(GPIOPortPISR)
DECLARE_ISR(GPIOPinP1ISR)
DECLARE_ISR(GPIOPinP2ISR)
DECLARE_ISR(GPIOPinP3ISR)
DECLARE_ISR(GPIOPinP4ISR)
DECLARE_ISR(GPIOPinP5ISR)
DECLARE_ISR(GPIOPinP6ISR)
DECLARE_ISR(GPIOPinP7ISR)
DECLARE_ISR(GPIOPortQISR)
DECLARE_ISR(GPIOPinQ1ISR)
DECLARE_ISR(GPIOPinQ2ISR)
DECLARE_ISR(GPIOPinQ3ISR)
DECLARE_ISR(GPIOPinQ4ISR)
DECLARE_ISR(GPIOPinQ5ISR)
DECLARE_ISR(GPIOPinQ6ISR)
DECLARE_ISR(GPIOPinQ7ISR)
DECLARE_ISR(GPIOPortRISR)
DECLARE_ISR(GPIOPortSISR)
DECLARE_ISR(PWM1Generator0ISR)
DECLARE_ISR(PWM1Generator1ISR)
DECLARE_ISR(PWM1Generator2ISR)
DECLARE_ISR(PWM1Generator3ISR)
DECLARE_ISR(PWM1FaultISR)

// Rest of the application
extern int main(void);

// Systemstack
static unsigned long pulStack[64];


// Vektor table, linker magic makes sure it lands at 0x00000000
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
    (void (*)(void))((unsigned long)pulStack + sizeof(pulStack)), // The initial stack pointer
    ResetISR, // The reset handler
    NmiSR, // The NMI handler
    FaultISR, // The hard fault handler
    MPUFaultISR, // The MPU fault handler
    BusFaultISR, // The bus fault handler
    UsageFaultISR, // The usage fault handler
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    SVCallISR, // SVCall handler
    DebugMonitorISR, // Debug monitor handler
    0, // Reserved
    PendSVISR, // The PendSV handler
    SysTickISR, // The SysTick handler
    GPIOPortAISR, // GPIO Port A
    GPIOPortBISR, // GPIO Port B
    GPIOPortCISR, // GPIO Port C
    GPIOPortDISR, // GPIO Port D
    GPIOPortEISR, // GPIO Port E
    UART0RxTxISR, // UART0 Rx and Tx
    UART1RxTxISR, // UART1 Rx and Tx
    SSIORxTxISR, // SSI0 Rx and Tx
    I2C0MasterSlaveISR, // I2C0 Master and Slave
    PWMFaultISR, // PWM Fault
    PWM0Generator0ISR, // PWM Generator 0
    PWM0Generator1ISR, // PWM Generator 1
    PWM0Generator2ISR, // PWM Generator 2
    QuadratureEncoder0ISR, // Quadrature Encoder 0
    ADC0Sequence0ISR, // ADC Sequence 0
    ADC0Sequnece1ISR, // ADC Sequence 1
    ADC0Sequnece2ISR, // ADC Sequence 2
    ADC0Sequnece3ISR, // ADC Sequence 3
    WatchdogTimerISR, // Watchdog timer
    Timer0AISR, // Timer 0 subtimer A
    Timer0BISR, // Timer 0 subtimer B
    Timer1AISR, // Timer 1 subtimer A
    Timer1BISR, // Timer 1 subtimer B
    Timer2AISR, // Timer 2 subtimer A
    Timer2BISR, // Timer 2 subtimer B
    AnalogComparator0ISR, // Analog Comparator 0
    AnalogComparator1ISR, // Analog Comparator 1
    AnalogComparator2ISR, // Analog Comparator 2
    SystemControlISR, // System Control (PLL, OSC, BO)
    FlashControlISR , // FLASH Control
    GPIOPortFISR, // GPIO Port F
    GPIOPortGISR, // GPIO Port G
    GPIOPortHISR, // GPIO Port H
    UART2RxTxISR, // UART2 Rx and Tx
    SSI1RxTxISR, // SSI1 Rx and Tx
    Timer3AISR, // Timer 3 subtimer A
    Timer3BISR, // Timer 3 subtimer B
    I2C1MasterSlaveISR, // I2C1 Master and Slave
    QuadratureEncoder1ISR, // Quadrature Encoder 1
    CAN0ISR, // CAN0
    CAN1ISR, // CAN1
    CAN2ISR, // CAN2
    EthernetISR, // Ethernet
    HibernateISR, // Hibernate
    USB0ISR, // USB0
    PWMGenerator3ISR, // PWM Generator 3
    uDMASoftwareTransferISR, // uDMA Software Transfer
    uDMAErrorISR, // uDMA Error
    ADC1Sequnece0ISR, // ADC1 Sequence 0
    ADC1Sequnece1ISR, // ADC1 Sequence 1
    ADC1Sequnece2ISR, // ADC1 Sequence 2
    ADC1Sequnece3ISR, // ADC1 Sequence 3
    I2S0ISR, // I2S0
    ExternalBusISR, // External Bus Interface 0
    GPIOPortJISR, // GPIO Port J
    GPIOPortKISR, // GPIO Port K
    GPIOPortLISR, // GPIO Port L
    SSI2RxTxISR, // SSI2 Rx and Tx
    SSI3RxTxISR, // SSI3 Rx and Tx
    UART3RxTxISR, // UART3 Rx and Tx
    UART4RxTxISR, // UART4 Rx and Tx
    UART5RxTxISR, // UART5 Rx and Tx
    UART6RxTxISR, // UART6 Rx and Tx
    UART7RxTxISR, // UART7 Rx and Tx
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    I2C2MasterSlaveISR, // I2C2 Master and Slave
    I2C3MasterSlaveISR, // I2C3 Master and Slave
    Timer4AISR, // Timer 4 subtimer A
    Timer4BISR, // Timer 4 subtimer B
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    0, // Reserved
    Timer5AISR, // Timer 5 subtimer A
    Timer5BISR, // Timer 5 subtimer B
    WideTimer0AISR, // Wide Timer 0 subtimer A
    WideTimer0BISR, // Wide Timer 0 subtimer B
    WideTimer1AISR, // Wide Timer 1 subtimer A
    WideTimer1BISR, // Wide Timer 1 subtimer B
    WideTimer2AISR, // Wide Timer 2 subtimer A
    WideTimer2BISR, // Wide Timer 2 subtimer B
    WideTimer3AISR, // Wide Timer 3 subtimer A
    WideTimer3BISR, // Wide Timer 3 subtimer B
    WideTimer4AISR, // Wide Timer 4 subtimer A
    WideTimer4BISR, // Wide Timer 4 subtimer B
    WideTimer5AISR, // Wide Timer 5 subtimer A
    WideTimer5BISR, // Wide Timer 5 subtimer B
    FPUISR, // FPU
    PECI0ISR, // PECI 0
    LPC0ISR, // LPC 0
    I2C4MasterSlaveISR, // I2C4 Master and Slave
    I2C5MasterSlaveISR, // I2C5 Master and Slave
    CPIOPortMISR, // GPIO Port M
    CPIOPortNISR, // GPIO Port N
    QuadratureEncoder2ISR, // Quadrature Encoder 2
    Fan0ISR, // Fan 0
    0, // Reserved
    GPIOPortPISR, // GPIO Port P (Summary or P0)
    GPIOPinP1ISR, // GPIO Port P1
    GPIOPinP2ISR, // GPIO Port P2
    GPIOPinP3ISR, // GPIO Port P3
    GPIOPinP4ISR, // GPIO Port P4
    GPIOPinP5ISR, // GPIO Port P5
    GPIOPinP6ISR, // GPIO Port P6
    GPIOPinP7ISR, // GPIO Port P7
    GPIOPortQISR, // GPIO Port Q (Summary or Q0)
    GPIOPinQ1ISR, // GPIO Port Q1
    GPIOPinQ2ISR, // GPIO Port Q2
    GPIOPinQ3ISR, // GPIO Port Q3
    GPIOPinQ4ISR, // GPIO Port Q4
    GPIOPinQ5ISR, // GPIO Port Q5
    GPIOPinQ6ISR, // GPIO Port Q6
    GPIOPinQ7ISR, // GPIO Port Q7
    GPIOPortRISR, // GPIO Port R
    GPIOPortSISR, // GPIO Port S
    PWM1Generator0ISR, // PWM 1 Generator 0
    PWM1Generator1ISR, // PWM 1 Generator 1
    PWM1Generator2ISR, // PWM 1 Generator 2
    PWM1Generator3ISR, // PWM 1 Generator 3
    PWM1FaultISR // PWM 1 Fault
};

// Pointers that will be filled in by the linker
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;



// The first thing be called on reset, copy data segment from flash to ram, zero bss segment
void DefaultResetISR(void)
{
    unsigned long *pulSrc, *pulDest, *bss;

    
    // Copy data segment initializers from flash to SRAM.
    pulSrc = &_etext;
    for(pulDest = &_data; pulDest < &_edata; )
    {
        *pulDest++ = *pulSrc++;
    }

    // Zero fill the bss segment.
    for(bss = &_bss; bss < &_ebss; bss++) {
        *bss = 0;
    }

}


// Hey there was an interrupt and you didn't write an ISR for it ... should not happen.
// Let's just wait until you attach an debugger.
void BadISR(void) {
    while(1)
    {
    }
}


// A lot of ugly copy pasta code

// Warning : Call DefaultResetISR() and main() when redefining this symbol
void ResetISR(void) {
    DefaultResetISR();
    main();
}