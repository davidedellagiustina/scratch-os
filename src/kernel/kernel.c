// @desc     Kernel
// @author   Davide Della Giustina
// @date     07/12/2019

#include <stdint.h>
#include "../cpu/isr.h"
#include "../cpu/paging.h"
#include "../drivers/vga.h"

/* Print ASCII art
 */
void print_ascii_art() {
    kprint(" ____                 _       _      ___  ____\n");
    kprint("/ ___|  ___ _ __ __ _| |_ ___| |__  / _ \\/ ___|\n");
    kprint("\\___ \\ / __| '__/ _` | __/ __| '_ \\| | | \\___ \\\n");
    kprint(" ___) | (__| | | (_| | || (__| | | | |_| |___) |\n");
    kprint("|____/ \\___|_|  \\__,_|\\__\\___|_| |_|\\___/|____/");
}

/* Kernel main.
 * @param kvs       Kernel start virtual address.
 * @param kve       Kernel end virtual address.
 * @param kps       Kernel start physical address.
 * @param kpe       Kernel end physical address.
 */
void kmain(void *kvs, void *kve, physaddr_t kps, physaddr_t kpe) {
    clear_screen();
    kprint("Booting ScratchOS v0.1...\n\n");
    // Print some info
    uint32_t kernel_size = ((kpe - kps) / 1024) - 4; // In KB, subtracting the size of kernel stack
    char buf[10]; itoa(kernel_size, buf, 10);
    kprint("Kernel approximate size: "); kprint(buf); kprint("KB.\n");
    // Install interrupt handlers
    kprint("Installing interrupts...");
    isr_install();
    irq_init();
    kprint(" Done!\n");
    // Setup paging
    kprint("Setting up paging...");
    // setup_paging(kvs, kve, kps, kpe);
    (void)kvs; (void)kve; (void)kps; (void)kpe; // Unused variables until paging is fixed
    kprint(" Done!\n");
    // TEMP: Basic shell-like interface here
    clear_screen();
    print_ascii_art();
    kprint("\n\n> ");
    // TEMP: Test page faults
    // uint32_t *ptr = (uint32_t *)0xa0000000;
    // uint32_t do_page_fault = *ptr;
    // (void)(do_page_fault); // Unused variable, just for test
}