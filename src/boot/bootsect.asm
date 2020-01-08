; @desc     Basic one-stage boot sector: load kernel from disk, switch to protected mode and transfer control to kernel
; @author   Davide Della Giustina
; @date     14/11/2019

[org 0x7c00]

KERNEL_OFFSET equ 0x1000

[bits 16]

jmp init

%include "src/boot/lib/16bit/print.asm"
%include "src/boot/lib/16bit/println.asm"
%include "src/boot/lib/16bit/hexprint.asm"
%include "src/boot/lib/16bit/loaddisk.asm"
%include "src/boot/gdt.asm"
%include "src/boot/lib/32bit/print.asm"

[bits 16] ; Real mode code

; Initialization
init:
    mov [BOOT_DRIVE], dl ; Boot drive number is stored by the BIOS, so save it
    mov bp, 0x9000 ; Stack
    mov sp, bp
    mov bx, BOOTING_OS_MSG ; Boot message
    call print
    call load_kernel ; Load kernel into memory
    call switch_to_pm ; Switch to protected mode

; Load kernel into memory
load_kernel:
    mov bx, KERNEL_OFFSET
    mov dh, KERNEL_SECTORS_SIZE ; This variable is defined by the assembler at compile-time
    mov dl, [BOOT_DRIVE]
    call loaddisk
    ret

; Switch to protected mode
switch_to_pm:
    cli ; Must stop receiving interrupts, will be re-enabled later
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:pm_init ; Long jump (i.e. other segment)

[bits 32] ; Protected mode code

; Protected mode initialization
pm_init:
    mov ax, DATA_SEG ; Segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000 ; Stack
    mov esp, ebp
    call main

; Main routine
main:
    call KERNEL_OFFSET
    jmp $

BOOT_DRIVE: db 0
BOOTING_OS_MSG: db 'Booting OwlOs...', 0

times 510-($-$$) db 0 ; Padding to 1 whole sector (512B)
dw 0xaa55 ; Magic number