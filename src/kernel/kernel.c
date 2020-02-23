// @desc     Kernel
// @author   Davide Della Giustina
// @date     07/12/2019

#include <stdint.h>
#include "../cpu/isr.h"
#include "../cpu/paging.h"
#include "../drivers/vga.h"

/* Kernel main.
 */
void kmain() {
    // Install interrupt handlers
    isr_install();
    irq_init();
    // Setup paging
    // init_paging();
    // Some basic tests
    clear_screen();
    kprint("OwlOS v0.1\n\n> ");
    // Test paging
    // uint32_t *ptr = (uint32_t *)0xa0000000;
    // uint32_t do_page_fault = *ptr;
    // (void)(do_page_fault); // Unused variable, just for test
}