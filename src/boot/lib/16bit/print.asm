; @desc     Real mode (16 bit) function: print a string
; @author   Davide Della Giustina
; @date     24/11/2019

[bits 16]

; @param bx     Pointer to (null-terminated) string to be printed
print:
    pusha
    mov ah, 0x0e
print_loop:
    mov al, [bx]
    cmp al, 0
    je print_exit
    int 0x10
    add bx, 1
    jmp print_loop
print_exit:
    popa
    ret