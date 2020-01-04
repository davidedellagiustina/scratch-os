// @desc     Interrupt Service Routines (ISR) header
// @author   Davide Della Giustina
// @date     31/12/2019

#ifndef ISR_H
#define ISR_H

#include "types.h"
#include "idt.h"
#include "../kernel/utils.h"
#include "../kernel/ports.h"
#include "../drivers/vga.h"

// PIC ports
#define PIC_MASTER 0x20
#define PIC_MASTER_COMMAND PIC_MASTER
#define PIC_MASTER_DATA (PIC_MASTER+1)
#define PIC_SLAVE 0xa0
#define PIC_SLAVE_COMMAND PIC_SLAVE
#define PIC_SLAVE_DATA (PIC_SLAVE+1)

// The first 32 ISRs are reserved for CPU exceptions
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
// The following 16 are IRQs
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

// Registers structure
typedef struct {
    uint32_t ds; // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t int_no, err_code; // Interrupt number and error code, if applicable
    uint32_t eip, cs, eflags, useresp, ss; // Automatically pushed by the CPU
} registers_t;

/* Remap the PICs to the desired offsets.
 * @param offset_master         New offset for the master PIC.
 * @param offset_slave          New offset for the slave PIC.
 */
void remap_pic(uint32_t offset_master, uint32_t offset_slave);

/* Setup IDT gate for every interrupt, then load IDT descriptor.
 */
void isr_install();

/* Generic ISR handler function.
 * @param r             Registers.
 */
void isr_handler(registers_t r);

typedef void (*isr_t)(registers_t); // 'isr_t' is a void function with a registers_t parameter

/* Register an IRQ handler
 * @param n                     Interrupt number.
 * @param handler               Handler function.
 */
void register_interrupt_handler(uint8_t n, isr_t handler);

#endif