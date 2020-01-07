// @desc     Kernel header
// @author   Davide Della Giustina
// @date     07/01/2020

#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include "../cpu/isr.h"
#include "../drivers/vga.h"
#include "../libc/string.h"

/* Parse basic shell commands.
 * @param cmd           Input command.
 */
void parse_input(char *cmd);

#endif