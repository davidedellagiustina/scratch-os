// @desc     Simple kernel
// @author   Davide Della Giustina
// @date     07/12/2019

#include "utils.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../drivers/vga.h"

void kmain() {
    isr_install(); // Install interrupts (i.e. load the IDT descriptor)
    clear_screen();
    kprint("Hello world!\n");
    __asm__ __volatile__("int $2");
    __asm__ __volatile__("int $3");
}