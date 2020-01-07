// @desc     Simple kernel
// @author   Davide Della Giustina
// @date     07/12/2019

#include "../cpu/isr.h"
#include "../drivers/vga.h"

void kmain() {
    // Install interrupt handlers
    isr_install();
    irq_install();
    // Some basic tests
    clear_screen();
    kprint("Hello world!\n");
}