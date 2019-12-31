// @desc     Interrupt Descriptor Table (IDT)
// @author   Davide Della Giustina
// @date     30/12/2019

#include "idt.h"

/* Set a certain interrupt handler in the IDT table.
 * @param n         Index inside the IDT (i.e. interrupt number).
 * @param handler   Address of the handler function for this interrupt.
 */
void set_idt_gate(uint32_t n, uint32_t handler) {
    idt[n].low_offset = low_16(handler);
    idt[n].sel = KERNEL_CS;
    idt[n].always_zero = 0;
    idt[n].flags = 0x8E; // 10001110b => interrupt present, kernel privileges, interrupt gate set, 32 bit gate
    idt[n].high_offset = high_16(handler);
}

/* Load the IDT in memory.
 */
void set_idt() {
    idt_reg.base = (uint32_t)&idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    __asm__ __volatile__("lidt (%0)" : : "r" (&idt_reg));
}