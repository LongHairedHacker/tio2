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
		gpio_a = 0x40058000;
		gpio_b = 0x40059000;
		gpio_c = 0x4005a000;
		gpio_d = 0x4005b000;
		gpio_e = 0x4005c000;
		gpio_f = 0x4005d000;
	}
}
