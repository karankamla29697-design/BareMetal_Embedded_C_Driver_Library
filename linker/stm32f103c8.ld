/* =========================================================
 * STM32F103C8 Linker Script
 * ========================================================= */

/* Entry point */
ENTRY(Reset_Handler)

/* =========================================================
 * Memory Layout
 * ========================================================= */

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

/* =========================================================
 * Stack
 * ========================================================= */

_estack = ORIGIN(RAM) + LENGTH(RAM);

/* =========================================================
 * Sections
 * ========================================================= */

SECTIONS
{
    /* Vector table + program code */
    .text :
    {
        . = ALIGN(4);

        KEEP(*(.isr_vector))

        *(.text)
        *(.text*)

        . = ALIGN(4);

        _etext = .;
    } > FLASH

    /* Read-only data */
    .rodata :
    {
        . = ALIGN(4);

        *(.rodata)
        *(.rodata*)

        . = ALIGN(4);
    } > FLASH

    /* Initialized data */
    .data :
    {
        . = ALIGN(4);

        _sdata = .;

        *(.data)
        *(.data*)

        . = ALIGN(4);

        _edata = .;
    } > RAM AT > FLASH

    /* Address of initialized data in Flash */
    _sidata = LOADADDR(.data);

    /* Zero-initialized data */
    .bss :
    {
        . = ALIGN(4);

        _sbss = .;

        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);

        _ebss = .;
    } > RAM
}