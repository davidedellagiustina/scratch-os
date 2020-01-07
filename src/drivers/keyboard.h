// @desc     Keyboard driver header
// @author   Davide Della Giustina
// @date     04/01/2020

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../cpu/isr.h"
#include "../cpu/ports.h"
#include "../cpu/types.h"
#include "../kernel/kernel.h"
#include "../libc/function.h"
#include "../libc/string.h"
#include "vga.h"

#define SCANCODE_PORT       0x60
#define KEYUP_EVENT_OFFSET  0x80

/* Initialize keryboard listener.
 */
void init_keyboard();

#endif