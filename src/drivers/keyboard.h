// @desc     Keyboard driver header
// @author   Davide Della Giustina
// @date     04/01/2020

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../cpu/types.h"
#include "../kernel/ports.h"
#include "vga.h"
#include "../cpu/isr.h"

#define SCANCODE_PORT 0x60
#define KEYUP_EVENT_OFFSET 0x80

/* Initialize keryboard listener.
 */
void init_keyboard();

#endif