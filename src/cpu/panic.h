// @desc     Panic header
// @author   Davide Della Giustina
// @date     16/02/2020

#ifndef PANIC_H
#define PANIC_H

#include <stdint.h>
#include "../drivers/vga.h"

/* Panic and halt the CPU with an error message.
 * @param msg       Error message to be printed.
 */
void panic(char *msg);

#endif