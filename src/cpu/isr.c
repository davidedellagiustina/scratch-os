// @desc     Interrupt Service Routines (ISR)
// @author   Davide Della Giustina
// @date     31/12/2019

#include "isr.h"

isr_t interrupt_handlers[IDT_ENTRIES];

/* Remap the PICs to the desired offsets.
 * @param offset_master         New offset for the master PIC.
 * @param offset_slave          New offset for the slave PIC.
 */
void remap_pic(uint32_t offset_master, uint32_t offset_slave) {
    // Save current masks
    uint8_t mask_master, mask_slave;
    mask_master = inb(PIC_MASTER_DATA);
    mask_slave = inb(PIC_SLAVE_DATA);
    // Initialize PICs
    outb(PIC_MASTER_COMMAND, 0x10 | 0x01); // Start initialization
    outb(PIC_SLAVE_COMMAND, 0x10 | 0x01);
    outb(PIC_MASTER_DATA, offset_master);
    outb(PIC_SLAVE_DATA, offset_slave);
    outb(PIC_MASTER_DATA, 4); // Tell master PIC that there is a slave at IRQ2
    outb(PIC_SLAVE_DATA, 2); // Tell slave PIC its identity
    outb(PIC_MASTER_DATA, 0x01); // 8086/88 mode
    outb(PIC_SLAVE_DATA, 0x01);
    // Restore masks
    // outb(PIC_MASTER_DATA, mask_master);
    // outb(PIC_SLAVE_DATA, mask_slave);
    outb(PIC_MASTER_DATA, 0x00);
    outb(PIC_SLAVE_DATA, 0x00);
}

/* Setup IDT gate for every interrupt, then load IDT descriptor.
 */
void isr_install() {
    // Register ISR handlers
    set_idt_gate(0, (uint32_t)isr0);
    set_idt_gate(1, (uint32_t)isr1);
    set_idt_gate(2, (uint32_t)isr2);
    set_idt_gate(3, (uint32_t)isr3);
    set_idt_gate(4, (uint32_t)isr4);
    set_idt_gate(5, (uint32_t)isr5);
    set_idt_gate(6, (uint32_t)isr6);
    set_idt_gate(7, (uint32_t)isr7);
    set_idt_gate(8, (uint32_t)isr8);
    set_idt_gate(9, (uint32_t)isr9);
    set_idt_gate(10, (uint32_t)isr10);
    set_idt_gate(11, (uint32_t)isr11);
    set_idt_gate(12, (uint32_t)isr12);
    set_idt_gate(13, (uint32_t)isr13);
    set_idt_gate(14, (uint32_t)isr14);
    set_idt_gate(15, (uint32_t)isr15);
    set_idt_gate(16, (uint32_t)isr16);
    set_idt_gate(17, (uint32_t)isr17);
    set_idt_gate(18, (uint32_t)isr18);
    set_idt_gate(19, (uint32_t)isr19);
    set_idt_gate(20, (uint32_t)isr20);
    set_idt_gate(21, (uint32_t)isr21);
    set_idt_gate(22, (uint32_t)isr22);
    set_idt_gate(23, (uint32_t)isr23);
    set_idt_gate(24, (uint32_t)isr24);
    set_idt_gate(25, (uint32_t)isr25);
    set_idt_gate(26, (uint32_t)isr26);
    set_idt_gate(27, (uint32_t)isr27);
    set_idt_gate(28, (uint32_t)isr28);
    set_idt_gate(29, (uint32_t)isr29);
    set_idt_gate(30, (uint32_t)isr30);
    set_idt_gate(31, (uint32_t)isr31);
    // Remap PIC
    remap_pic(0x20, 0x28); // Remap master PIC to 0x20-0x27, slave PIC to 0x28-0x2f
    // Register IRQ handlers
    set_idt_gate(32, (uint32_t)irq0);
    set_idt_gate(33, (uint32_t)irq1);
    set_idt_gate(34, (uint32_t)irq2);
    set_idt_gate(35, (uint32_t)irq3);
    set_idt_gate(36, (uint32_t)irq4);
    set_idt_gate(37, (uint32_t)irq5);
    set_idt_gate(38, (uint32_t)irq6);
    set_idt_gate(39, (uint32_t)irq7);
    set_idt_gate(40, (uint32_t)irq8);
    set_idt_gate(41, (uint32_t)irq9);
    set_idt_gate(42, (uint32_t)irq10);
    set_idt_gate(43, (uint32_t)irq11);
    set_idt_gate(44, (uint32_t)irq12);
    set_idt_gate(45, (uint32_t)irq13);
    set_idt_gate(46, (uint32_t)irq14);
    set_idt_gate(47, (uint32_t)irq15);
    // Load IDT in memory
    set_idt();
}

// Excptions messages
char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out Of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    //
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bas TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    //
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    //
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

/* Generic ISR handler function.
 * @param r             Registers.
 */
void isr_handler(registers_t r) {
    kprint("Interrupt ");
    char s[3];
    int_to_ascii(r.int_no, s);
    kprint(s);
    kprint(": ");
    kprint(exception_messages[r.int_no]);
    kprint("\n");
}

/* Register an IRQ handler
 * @param n                     Interrupt number.
 * @param handler               Handler function.
 */
void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t r) {
    if (r.int_no >= 40) outb(PIC_SLAVE_COMMAND, 0x20); // EOI (End Of Interrupt)
    outb(PIC_MASTER_COMMAND, 0x20);
    if (interrupt_handlers[r.int_no] != 0) {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    }
}