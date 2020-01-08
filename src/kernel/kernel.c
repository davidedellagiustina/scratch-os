// @desc     Kernel
// @author   Davide Della Giustina
// @date     07/12/2019

#include "kernel.h"

/* Kernel main.
 */
void kmain() {
    // Install interrupt handlers
    isr_install();
    irq_init();
    // Some basic tests
    clear_screen();
    kprint("Hello world!\n> ");
}

/* Parse basic shell commands.
 * @param cmd           Input command.
 */
void parse_input(char *cmd) {
    if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0) {
        kprint("Halting the CPU...");
        asm volatile("hlt");
    } else if (strcmp(cmd, "mkpage") == 0) {
        uint32_t page, physical;
        page = kmalloc(1000, 1, &physical);
        char page_str[16] = "", physical_str[16] = "";
        itoa(page, page_str, 16);
        itoa(physical, physical_str, 16);
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(physical_str);
    } else {
        kprint("Unrecognised command");
    }
    kprint("\n> ");
}