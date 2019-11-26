; @desc     Real mode (16 bit) function: load data from disk
; @author   Davide Della Giustina
; @date     26/11/2019

[bits 16]

; @param dh     Number of sectors to read
; @param dl     Drive number
; @param es:bx  Destination address
loaddisk:
    pusha
    push dx ; DL -> drive number (from BIOS)
    mov ah, 0x02 ; BIOS read disk function
    mov al, dh ; AL -> number of sectors to read
    mov ch, 0x00 ; CH -> cylinder
    mov dh, 0x00 ; DH -> head
    mov cl, 0x02 ; CL -> sector
    int 0x13
    jc loaddisk_general_error
    pop dx
    cmp dh, al
    jne loaddisk_sectors_error
    popa
    ret
loaddisk_general_error:
    mov bx, LOADDISK_GENERAL_ERROR_MSG
    call print
    jmp $
loaddisk_sectors_error:
    mov bx, LOADDISK_SECTORS_ERROR_MSG
    call print
    jmp $

LOADDISK_SECTORS_ERROR_MSG: db 'Disk error: invalid number of sectors.', 0
LOADDISK_GENERAL_ERROR_MSG: db 'Disk error: could not load data.', 0