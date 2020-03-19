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
global read_eip

; Higher-half kernel start
; Delete identity mapping, load labels and then call C code
; Also reserve extra space for kernel stack
section .text
kernel_entry:
    pop eax ; EAX -> GDT descriptor virtual address
    mov edx, eax ; Rewrite GDT descriptor 2nd entry (i.e. GDT base address) with virtual address
    add edx, 2
    mov ebx, [edx]
    add ebx, 0xc0000000
    mov [edx], ebx
    lgdt [eax] ; Reload GDT descriptor register (with virtual address)
    mov ebp, KERNEL_STACK + KERNEL_STACK_SIZE ; Setup new kernel stack
    mov esp, ebp
    mov eax, cr3 ; Get rid of identity-mapping
    mov edx, 0
    mov [eax], edx
    invlpg [eax] ; Invalidate TLB
    push kernel_physical_end ; Push labels to stack
    push kernel_physical_start
    push kernel_virtual_end
    push kernel_virtual_start
    call kmain ; Call kmain
    jmp $ ; Control should never get back there, just for prevention

; Read the instruction pointer
read_eip:
    pop eax
    jmp eax

section .bss
align 4
KERNEL_STACK: resb KERNEL_STACK_SIZE ; Reserve space for kernel stack