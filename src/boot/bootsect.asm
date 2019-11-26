; @desc     Basic boot sector: load kernel from disk, witch to protected mode and transfer control to kernel.
; @author   Davide Della Giustina
; @date     14/11/2019

[org 0x7c00]

[bits 16]

; Initialization stuff
mov [BOOT_DRIVE], dl ; Boot drive number is stored by the BIOS
mov bp, 0x8000 ; Stack
mov sp, bp
jmp main

%include "src/boot/libs/16bit/print.asm"
%include "src/boot/libs/16bit/println.asm"
%include "src/boot/libs/16bit/hexprint.asm"
%include "src/boot/libs/16bit/loaddisk.asm"

; Main routine
main:
    mov bx, 0x9000
    mov dh, 2
    mov dl, [BOOT_DRIVE]
    call loaddisk
    mov dx, [0x9000]
    call hexprint
    call println
    mov dx, [0x9000 + 512]
    call hexprint
    jmp $

BOOT_DRIVE: db 0

; 238 bytes till here

times 510-($-$$) db 0
dw 0xaa55 ; Magic number

times 256 dw 0xdada
times 256 dw 0xface