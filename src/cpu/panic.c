// @desc     Panic header
// @author   Davide Della Giustina
// @date     16/02/2020

#include "panic.h"

/* Panic and halt the CPU with an error message.
 * @param msg       Error message to be printed.
 */
void panic(char *msg) {
    asm volatile("cli"); // Disable interrupts
    kprint("*** System panic: ");
    kprint(msg);
    kprint(" ***\nHalting the CPU...");
    asm volatile("hlt"); // Halt the CPU
}