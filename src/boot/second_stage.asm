; @desc     Second stage bootloader: enable paging
; @author   Davide Della Giustina
; @date     23/02/2020

[bits 32]

KERNEL_VIRTUAL_BASE equ 0xc0000000 ; Virtual base address for kernel section
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22) ; Page table number of kernel

global second_stage_bootloader

; Second stage bootloader: enable paging, identity-map first 4MB, relocate kernel at 0xc0000000
second_stage_bootloader:
    pusha
    call set_page_tbl ; Setup page table nr.0
    call set_page_dir ; Setup boot page directory
    mov eax, BOOT_PAGE_DIR ; load boot page directory address in cr3 register
    mov cr3, eax
    mov eax, cr0 ; Enable paging (set PG bit in cr0 register)
    or eax, 0x80000000
    mov cr0, eax
    popa
    ret

; Setup page table nr.0
; (Identity-map for first 4MB of memory)
set_page_tbl:
    pusha
    mov ecx, 0 ; ECX -> Current page table entry number
    mov edx, BOOT_PAGE_TBL ; EDX --> Pointer to page table entry
pgtbl_loop:
    cmp ecx, 1024 ; If already set 1024 entries
    je pgtbl_exit ; Then exit loop
    mov eax, ecx ; Otherwise EAX = entry number
    shl eax, 12 ; Set frame address (i.e. entry number << 12)
    or eax, 0x3 ; Set presence bit
    mov [edx], eax ; Set page table entry
    add ecx, 1 ; Increment entry number
    add edx, 4 ; Next page table entry
    jmp pgtbl_loop ; Loop
pgtbl_exit:
    popa
    ret

; Setup boot page directory
; (Identity-map the first 4MB of memory + virtual kernel page table points to first 4MB of memory (i.e. physical kernel location))
set_page_dir:
    pusha
    mov edx, BOOT_PAGE_DIR ; EDX -> Pointer to page directory entry
    mov eax, [edx] ; Set page table nr.0 address (identity-mapping)
    or eax, 0x3 ; Presence bit
    mov [edx], eax
    add edx, 4*KERNEL_PAGE_NUMBER ; PD entry for virtual kernel's PT
    mov eax, [edx] ; Set page table for kernel virtual address
    or eax, 0x3 ; Presence bit
    mov [edx], eax
    popa
    ret

align 0x1000 ; Page tables must be aligned at 0x1000
BOOT_PAGE_TBL: ; Placeholder for page table nr.0 (supported max kernel size: 4MB - 0x3500)
    times 1024 dd 0 ; Will be filled later

align 0x1000 ; Page directory should be aligned at 0x1000
BOOT_PAGE_DIR: ; Placeholder for page directory
    dd BOOT_PAGE_TBL ; Identity-map page table (needed for executing code before jumping top the kernel)
    times (KERNEL_PAGE_NUMBER - 1) dd 0 ; Not interested in these page tables
    dd BOOT_PAGE_TBL ; Virtual kernel page table
    times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0 ; Not interested in these page tables

times 512-(($-$$) % 512) db 0 ; Padding to the end of the sector (required for loading in memory in real mode)