; @desc     Kernel entry: kall kmain()
; @author   Davide Della Giustina
; @date     07/12/2019

[bits 32]

KERNEL_STACK_SIZE equ 4*1024 ; In bytes

[extern kernel_virtual_start]
[extern kernel_virtual_end]
[extern kernel_physical_start]
[extern kernel_physical_end]
[extern kmain]

global kernel_entry

; Higher-half kernel start
; Delete identity mapping, load labels and then call C code
; Also reserve extra space for kernel stack
section .text
kernel_entry:
    mov ebp, KERNEL_STACK + KERNEL_STACK_SIZE ; Setup stack
    mov esp, ebp
    mov eax, cr3 ; Get rid of identity-mapping
    mov edx, 0
    mov [eax], edx
    push kernel_physical_end ; Push labels to stack
    push kernel_physical_start
    push kernel_virtual_end
    push kernel_virtual_start
    call kmain ; Call kmain
    jmp $ ; Control should never get back there, just for prevention

section .bss
align 4
KERNEL_STACK: resb KERNEL_STACK_SIZE ; Reserve space for kernel stack