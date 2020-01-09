// @desc     Keyboard driver header
// @author   Davide Della Giustina
// @date     04/01/2020

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include "../cpu/isr.h"
#include "../cpu/ports.h"
#include "../kernel/kernel.h"
#include "../libc/string.h"
#include "vga.h"

/* Initialize keryboard listener.
 */
void init_keyboard();

#endif