; @desc     Real mode (16 bit) function: print a newline character
; @author   Davide Della Giustina
; @date     24/11/2019

[bits 16]

println:
    pusha
    mov ah, 0x0e
    mov al, 0x0a ; 0x0a -> LF
    int 0x10
    mov al, 0xd ; 0x0d -> CR
    int 0x10
    popa
    ret