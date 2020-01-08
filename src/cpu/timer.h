// @desc     Timer header
// @author   Davide Della Giustina
// @date     04/01/2020

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "../drivers/vga.h"
#include "../libc/string.h"
#include "isr.h"

/* Initialize the CPU timer.
 * @param freq              Frequency.
 */
void init_timer(uint32_t freq);

#endif