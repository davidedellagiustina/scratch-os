// @desc     Interrupt Descriptor Table (IDT) header
// @author   Davide Della Giustina
// @date     30/12/2019

#ifndef IDT_H
#define IDT_H

#include "types.h"
#include "../kernel/utils.h"

#define KERNEL_CS 0x08 // Kernel code segment, defined in the first GDT entry (note: the 0x00 entry in GDT is null, so the very first is located at 0x08)
#define IDT_ENTRIES 256 // Must be 256, otherwise the CPU panics

// Interrupt gate
typedef struct {
    uint16_t low_offset; // Lower 16 bits of the handler function address
    uint16_t sel; // Kernel segment selector
    uint8_t zero; // Unused, has to be zero
    uint8_t flags;
    /* Flags:
     * Bit 7    ->  interrupt presence bit (set to 0 for unused interrupts)
     * Bits 6-5 ->  privilege of caller (0 => kernel .. 3 => user)
     * Bit 4    ->  set to 0 for interrupt and trap gates
     * Bits 3-0 ->  bits 1110 (14 decimal) = 32 bit interrupt gate (see https://wiki.osdev.org/Interrupt_Descriptor_Table for further details)
     */
    uint16_t high_offset; // Higher 16 bits of the handler function address
} __attribute__((packed)) idt_gate_t;

// IDT descriptor (loaded by 'lidt' assembly instruction)
typedef struct {
    uint16_t limit; // Length of IDT
    uint32_t base; // Base address of IDT in memory
} __attribute__((packed)) idt_register_t;

// IDT table and descriptor definition
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

/* Set a certain interrupt handler in the IDT table.
 * @param n         Index inside the IDT (i.e. interrupt number).
 * @param handler   Address of the handler function for this interrupt.
 */
void set_idt_gate(uint32_t n, uint32_t handler);

/* Load the IDT in memory.
 */
void set_idt();

#endif