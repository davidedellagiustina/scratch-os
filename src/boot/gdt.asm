; @desc     GDT structure
; @author   Davide Della Giustina
; @date     27/11/2019

[bits 16]

gdt_start:

gdt_null:
    dd 0x0
    dd 0x0

gdt_code:
    ; Base -> 0x0 | Limit -> 0xfffff
    ; 1st flags: present=1, privilege=00, descriptor-type=1 -> 1001b
    ; Type flags: code=1, conforming=0, readable=1, accessed=0 -> 1010b
    ; 2nd flags: granularity=1, 32bit-default=1, 64bit-seg=0, avl=0 -> 1100b
    dw 0xffff ; Limit (0-15)
    dw 0x0 ; Base (0-15)
    db 0x0 ; Base (16-23)
    db 10011010b ; 1st flags + type flags
    db 11001111b ; 2nd flags + limit (16-19)
    db 0x0 ; Base (24-31)

gdt_data:
    ; Base -> 0x0 | Limit -> 0xfffff
    ; 1st flags: present=1, privilege=00, descriptor-type=1 -> 1001b
    ; Type flags: code=0, expand-down=0, writable=1, accessed=0 -> 0010b
    ; 2nd flags: granularity=1, 32bit-default=1, 64bit-seg=0, avl=0 -> 1100b
    dw 0xffff ; Limit (0-15)
    dw 0x0 ; Base (0-15)
    db 0x0 ; Base (16-23)
    db 10010010b ; 1st flags + type flags
    db 11001111b ; 2nd flags + limit (16-19)
    db 0x0 ; Base (24-31)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; One less than real size of the GDT
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start