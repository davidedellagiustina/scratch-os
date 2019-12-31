// @desc     Interrupt Service Routines (ISR) header
// @author   Davide Della Giustina
// @date     31/12/2019

#ifndef ISR_H
#define ISR_H

#include "types.h"
#include "idt.h"
#include "../kernel/utils.h"
#include "../drivers/vga.h"

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

// Registers structure
typedef struct {
    uint32_t ds; // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t int_no, err_code; // Interrupt number and error code, if applicable
    uint32_t eip, cs, eflags, useresp, ss; // Automatically pushed by the CPU
} registers_t;

/* Setup IDT gate for every interrupt, then load IDT descriptor.
 */
void isr_install();

/* Generic ISR handler function.
 * @param r             Registers.
 */
void isr_handler(registers_t r);

#endif