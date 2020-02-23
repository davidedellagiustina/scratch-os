; @desc     Second stage bootloader: enable paging
; @author   Davide Della Giustina
; @date     23/02/2020

[bits 32]

second_stage_bootloader:
    ret

times 512-(($-$$) % 512) db 0 ; Padding to the end of the sector