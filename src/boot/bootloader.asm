; @desc     Basic bootloader: load kernel from disk, witch to protected mode and transfer control to kernel.
; @author   Davide Della Giustina
; @date     14/11/2019

[bits 16]
jmp $

times 510-($-$$) db 0
dw 0xaa55