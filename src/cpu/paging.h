// @desc     Paging header
// @author   Davide Della Giustina
// @date     16/02/2020

#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include "../drivers/vga.h"
#include "../kernel/heap.h"
#include "../libc/mem.h"
#include "isr.h"
#include "panic.h"

// Page table entry (4 bytes)
typedef struct {
        uint32_t present : 1; // Page is present in memory if set
        uint32_t rw : 1; // Page is writable if set
        uint32_t user : 1; // Page is user-mode if set
        uint32_t reserved_1 : 2; // Reserved for internal use, cannot be modified
        uint32_t accessed : 1; // Page has been accessed since last refresh if set (set by CPU)
        uint32_t dirty : 1; // Page has been written since last refresh if set
        uint32_t reserved_2 : 2; // Reserved for internal use, cannot be modified
        uint32_t unused : 3; // Unused bits, available for kernel use
        uint32_t frame_addr : 20; // Frame address
} page_t;

// Page table (array of 1024 pages)
typedef struct {
    page_t pages[1024];
} page_table_t;

// Page directory (array of 1024 page tables [pointers!])
typedef struct {
    uint32_t tables_physical[1024];
    page_table_t *tables[1024];
    physaddr_t physical_addr;
} page_directory_t;

/* Setup paging environment.
 * (New page directory, different from the boot one, with no identity mapping).
 * @param kvs       Pointer to virtual kernel start.
 * @param kve       Pointer to virtual kernel end.
 * @param kps       Physical address of kernel start.
 * @param kpe       Physical address of kernel end.
 */
void setup_paging(void *kvs, void *kve, physaddr_t kps, physaddr_t kpe);

/* Load a new page directory into the CR3 register.
 * @param page_directory        Address of the new page directory to load.
 */
void switch_page_directory(page_directory_t *page_directory);

/* Page faults handler function.
 * @param r                     Registers.
 */
void page_fault_handler(registers_t *r);

#endif