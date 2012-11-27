MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x00040000
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
}

ENTRY(ResetISR);

SECTIONS
{
    .text :
    {
        _text = .;
		/* place initial stack pointer */
		LONG(ORIGIN(SRAM) + LENGTH(SRAM) - 16*4);
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata*)

		. = ALIGN(4);
		_startup_hooks = .;
		KEEP(*(.startup_hook))
		_startup_hooks_end = .;
        
		_etext = .;
	} > FLASH

    .data : AT(ADDR(.text) + SIZEOF(.text))
    {
        _data = .;
        *(vtable)
        *(.data*)
        _edata = .;
    } > SRAM

    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM

	.peripherals :
	{
		_ZN4tio26gpio_aE = 0x40058000;
		_ZN4tio26gpio_bE = 0x40059000;
		_ZN4tio26gpio_cE = 0x4005a000;
		_ZN4tio26gpio_dE = 0x4005b000;
		_ZN4tio26gpio_eE = 0x4005c000;
		_ZN4tio26gpio_fE = 0x4005d000;

		_ZN4tio216gpio_reset_stateE = 0x400FE508;
		_ZN4tio211gpio_in_runE = 0x400FE608;
		_ZN4tio213gpio_in_sleepE = 0x400FE708;
		_ZN4tio218gpio_in_deep_sleepE = 0x400FE808;
		_ZN4tio211gpio_statusE = 0x400FEA08;

		_ZN4tio25clockE = 0x400FE000;
	}
}
