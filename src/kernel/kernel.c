// @desc     Kernel
// @author   Davide Della Giustina
// @date     07/12/2019

#include "../cpu/isr.h"
#include "../drivers/vga.h"
#include "../libc/string.h"

/* Kernel main.
 */
void kmain() {
    // Install interrupt handlers
    isr_install();
    irq_install();
    // Some basic tests
    clear_screen();
    kprint("Hello world!\n> ");
}

/* Parse basic shell commands.
 * @param input         Input command.
 */
void parse_input(char *input) {
    if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
        kprint("Halting the CPU...\n");
        asm volatile("hlt");
    } else {
        kprint("Unrecognised command\n> ");
    }
}