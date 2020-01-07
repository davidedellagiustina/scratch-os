// @desc     Timer
// @author   Davide Della Giustina
// @date     04/01/2020

#include "timer.h"

uint32_t tick = 0;

/* Handler for the timer interrupts.
 * @param r             CPU state (registers).
 */
static void timer_callback(registers_t r) {
    ++tick;
    UNUSED(r);
}

/* Initialize the CPU timer.
 * @param freq              Frequency.
 */
void init_timer(uint32_t freq) {
    register_interrupt_handler(IRQ0, timer_callback);
    uint32_t divisor = 1193180 / freq;
    uint8_t low = (uint8_t)(divisor & 0xff);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xff);
    outb(0x43, 0x36);
    outb(0x40, low);
    outb(0x40, high);
}