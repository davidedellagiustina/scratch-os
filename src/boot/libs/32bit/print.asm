; @desc     Protected mode (32 bit) function: print a string
; @author   Davide Della Giustina
; @date     27/11/2019

[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

; @param ebx        Pointer to (null-terminated) string to be printed
pm_print:
    pusha
    mov edx, VIDEO_MEMORY
pm_print_loop:
    mov al, [ebx]
    mov ah, WHITE_ON_BLACK
    cmp al, 0
    je pm_print_exit
    mov [edx], ax
    add ebx, 1
    add edx, 2
    jmp pm_print_loop
pm_print_exit:
    popa
    ret