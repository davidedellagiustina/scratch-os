; @desc     Real mode (16 bit) function: print a hexadecimal value
; @author   Davide Della Giustina
; @date     26/11/2019

[bits 16]

; @param dx
hexprint:
    pusha
    mov cx, 4
hexprint_loop:
    cmp cx, 0
    je hexprint_exit
    mov ax, dx
    and ax, 0x000f
    add al, 0x30 ; Hex digit to ASCII value
    cmp al, 0x39 ; If decimal digit
    jle hexprint_tobuf
    add al, 0x27 ; If letter
hexprint_tobuf:
    mov bx, HEXPRINT_OUT + 1
    add bx, cx
    mov [bx], al
    ror dx, 4
    sub cx, 1
    jmp hexprint_loop
hexprint_exit:
    sub bx, 2 ; BX now holds HEXPRINT_OUT address
    call print
    popa
    ret

HEXPRINT_OUT: db '0x0000', 0