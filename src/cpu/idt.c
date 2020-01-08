// @desc     Interrupt Descriptor Table (IDT)
// @author   Davide Della Giustina
// @date     30/12/2019

#include "idt.h"

/* Set a certain interrupt handler in the IDT table.
 * @param n         Index inside the IDT (i.e. interrupt number).
 * @param handler   Address of the handler function for this interrupt.
 */
void set_idt_gate(int n, uint32_t handler) {
    idt[n].low_offset = (uint16_t)(handler & 0xffff);
    idt[n].sel = KERNEL_CS;
    idt[n].zero = 0;
    idt[n].flags = 0x8e; // 10001110b => interrupt present, kernel privileges, interrupt gate set, 32 bit gate
    idt[n].high_offset = (uint16_t)((handler >> 16) & 0xffff);
}

/* Load the IDT in memory.
 */
void load_idt() {
    idt_reg.base = (uint32_t)&idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1; // 256 entries
    asm volatile("lidt (%0)" : : "r" (&idt_reg)); // Load IDT with 'lidt' instruction
}