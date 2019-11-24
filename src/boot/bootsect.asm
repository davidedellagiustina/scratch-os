; @desc     Basic boot sector: load kernel from disk, witch to protected mode and transfer control to kernel.
; @author   Davide Della Giustina
; @date     14/11/2019

[org 0x7c00]

[bits 16]

jmp main

%include "src/boot/libs/16bit/print.asm"
%include "src/boot/libs/16bit/println.asm"

main:
    mov bx, hello_msg
    call print
    call println
    jmp $

hello_msg:
    db 'Hello world!', 0

times 510-($-$$) db 0
dw 0xaa55 ; Magic number