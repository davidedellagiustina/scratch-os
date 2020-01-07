; @desc     Interrupt definitions
; @author   Davide Della Giustina
; @date     31/12/2019

; VERY USEFUL MACROS

; For ISRs that do not push an error code onto the stack.
; @param 1          ISR number.
%macro ISR_NOERR 1
    global isr%1
    isr%1:
        push byte 0 ; err_code
        push byte %1 ; err_no
        jmp isr_common_stub
%endmacro

; For ISRs that do push an error code onto the stack.
; @param 1          ISR number.
%macro ISR_ERR 1
    global isr%1
    isr%1:
        push byte %1
        jmp isr_common_stub
%endmacro

; For IRQs.
; @param 1          IRQ number.
; @param 2          Number the IRQ is remapped to.
%macro IRQ 2
    global irq%1
    irq%1:
        push byte %1
        push byte %2
        jmp irq_common_stub
%endmacro

[bits 32]
[extern isr_handler] ; Defined in isr.c
[extern irq_handler] ; Defined in isr.c

isr_common_stub:
    ; 1) Save the CPU state
    pusha ; Push edi, esi, ebp, esp, ebx, edx, ecx, eax
    mov ax, ds
    push eax ; Save data segment descriptor
    mov ax, 0x10 ; Kernel data segment descriptor, defined in GDT (entry #2, excluding the very first null entry)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp ; Pass by reference
    ; 2) Call generic C handler function
    call isr_handler
    ; 3) Restore state
    pop eax
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8 ; Clean up the pushed error code and ISR number
    iret ; Pops cs, eip, eflags, ss, esp, then returns from interrupt

irq_common_stub:
    ; 1) Save the CPU state
    pusha ; Push edi, esi, ebp, esp, ebx, edx, ecx, eax
    mov ax, ds
    push eax ; Save data segment descriptor
    mov ax, 0x10 ; Kernel data segment descriptor, defined in GDT (entry #2, excluding the very first null entry)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp ; Pass by reference
    ; 2) Call generic C handler function
    call irq_handler
    ; 3) Restore state
    pop ebx
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8 ; Clean up the pushed error code and ISR number
    iret ; Pops cs, eip, eflags, ss, esp, then returns from interrupt

; ISRs
ISR_NOERR 0 ; Division By Zero Exception
ISR_NOERR 1 ; Debug Exception
ISR_NOERR 2 ; Non Maskable Interrupt Exception
ISR_NOERR 3 ; Breakpoint Exception
ISR_NOERR 4 ; Into Detected Overflow Exception
ISR_NOERR 5 ; Out Of Bounds Exception
ISR_NOERR 6 ; Invalid Opcode Exception
ISR_NOERR 7 ; No Coprocessor Exception
ISR_ERR 8 ; Double Fault Exception (with error code)
ISR_NOERR 9 ; Coprocessor Segment Overrun Exception
ISR_ERR 10 ; Bas TSS Exception (with error code)
ISR_ERR 11 ; Segment Not Present Exception (with error code)
ISR_ERR 12 ; Stack Fault Exception (with error code)
ISR_ERR 13 ; General Protection Fault Exception (with error code)
ISR_ERR 14 ; Page Fault Exception (with error code)
ISR_NOERR 15 ; Unknown Interrupt Exception
ISR_NOERR 16 ; Coprocessor Fault Exception
ISR_NOERR 17 ; Alignment Check Exception
ISR_NOERR 18 ; Machine Check Exception
ISR_NOERR 19 ; Reserved Exception
ISR_NOERR 20 ; Reserved Exception
ISR_NOERR 21 ; Reserved Exception
ISR_NOERR 22 ; Reserved Exception
ISR_NOERR 23 ; Reserved Exception
ISR_NOERR 24 ; Reserved Exception
ISR_NOERR 25 ; Reserved Exception
ISR_NOERR 26 ; Reserved Exception
ISR_NOERR 27 ; Reserved Exception
ISR_NOERR 28 ; Reserved Exception
ISR_NOERR 29 ; Reserved Exception
ISR_NOERR 30 ; Reserved Exception
ISR_NOERR 31 ; Reserved Exception

; IRQs
IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47