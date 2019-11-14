; @desc     Basic boot sector: load kernel from disk, witch to protected mode and transfer control to kernel.
; @author   Davide Della Giustina
; @date     14/11/2019

[bits 16]

mov ah, 0x0e
mov al, 'H'
int 0x10
mov al, 'e'
int 0x10
mov al, 'l'
int 0x10
int 0x10
mov al, 'o'
int 0x10
mov al, '!'
int 0x10

jmp $

times 510-($-$$) db 0
dw 0xaa55 ; Magic number