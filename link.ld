/* @desc		Kernel linker script */
/* @author      Davide Della Giustina */
/* @date		23/02/2020 */

ENTRY(kernel_entry) /* Kernel entry label */

SECTIONS {
    . = 0xc0008000; /* Kernel code is relocated at 3GB + 0x8000 */

    kernel_virtual_start = .; /* Export labels */
    kernel_physical_start = . - 0xc0000000;

    .text ALIGN (0x1000) : AT(ADDR(.text)-0xc0000000) /* Align at 4KB and load at 4KB */
    {
        *(.text) /* All text sections from all files */
    }

    .rodata ALIGN (0x1000) : AT(ADDR(.rodata)-0xc0000000) /* Align at 4KB and load at 4KB */
    {
        *(.rodata*) /* All read-only data sections from all files */
    }

    .data ALIGN (0x1000) : AT(ADDR(.data)-0xc0000000) /* Align at 4KB and load at 4KB */
    {
        *(.data) /* All data sections from all files */
    }

    .bss ALIGN (0x1000) : AT(ADDR(.bss)-0xc0000000) /* Align at 4KB and load at 4KB */
    {
        *(COMMON) /* All COMMON sections from all files */
        *(.bss) /* All bss sections from all files */
    }

    kernel_virtual_end = .; /* Export labels */
    kernel_physical_end = . - 0xc0000000;
}