// @desc     Paging
// @author   Davide Della Giustina
// @date     16/02/2020

#include "paging.h"

uint32_t *frames; // Bitmap of frames
uint32_t nframes;

physaddr_t boot_directory; // Backup of the boot directory
page_directory_t *kernel_directory, *current_directory;

#define INDEX(x)        (x / 32)
#define OFFSET(x)       (x % 32)

// Private functions

static void set_frame(physaddr_t frame_address);
static void clear_frame(physaddr_t frame_address);
// static uint32_t test_frame(physaddr_t frame_address);
static uint32_t first_free_frame();
void alloc_frame(page_t *page, int is_kernel, int is_writable);
void free_frame(page_t *page);
void create_page_table(page_directory_t *page_directory, uint32_t index, uint8_t is_kernel, uint8_t is_writable);

// Public functions

/* Setup paging environment.
 * (New page directory, different from the boot one, with no identity mapping).
 * @param kvs       Pointer to virtual kernel start.
 * @param kve       Pointer to virtual kernel end.
 * @param kps       Physical address of kernel start.
 * @param kpe       Physical address of kernel end.
 */
void setup_paging(void *kvs, void *kve, physaddr_t kps, physaddr_t kpe) {
    // Backup boot page directory
    asm volatile("mov %%cr3, %0" : "=r"(boot_directory));
    // Register page fault handler (in order to detect possible page faults from now)
    register_interrupt_handler(14, page_fault_handler);
    // Allocate frame bitmap
    uint32_t mem_size = TOTAL_RAM_SIZE * 0x100000; // RAM size (in MB) * 1MB (argument passed at compile time)
    nframes = mem_size / 0x1000;
    frames = (uint32_t *)dumb_kcalloc(INDEX(nframes), 0, 0); // Allocate bitmap
    // Allocate page directory and tables before video memory
    kbrk(kve);
    physaddr_t phys;
    kernel_directory = (page_directory_t *)dumb_kcalloc(sizeof(page_directory_t), 1, &phys); // Allocate space for page directory;
    kernel_directory->physical_addr = phys;
    // Map boot + GDT + kernel + kernel dumb heap + video memory
    physaddr_t physaddr = 0x0;
    void *virtaddr = (void *)(physaddr + 0xc0000000);
    while (physaddr < 0xc0000) {
        uint32_t pti = (uint32_t)virtaddr >> 22; // Page table index
        uint32_t pi = ((uint32_t)virtaddr >> 12) & 0x3ff; // Page index
        if (!kernel_directory->tables[pti]) {
            create_page_table(kernel_directory, pti, 1, 1);
            kpe += sizeof(page_table_t);
        }
        kernel_directory->tables[pti]->pages[pi].present = 1; // Page is now present in memory
        kernel_directory->tables[pti]->pages[pi].rw = 1; // Page is writable
        kernel_directory->tables[pti]->pages[pi].user = 0; // Page is kernel mode
        kernel_directory->tables[pti]->pages[pi].frame_addr = (physaddr / 0x1000);
        set_frame(physaddr);
        physaddr += 0x1000; virtaddr += 0x1000; // Increment pointers
    }
    // Map some space for future kernel heap (63.25MB) -> Kernel heap limit is 64MB (0x4000000) [edit: removed last page frame, which is now used for temp mapping for fork()]
    while (physaddr < 0x3fff000) {
        uint32_t pti = (uint32_t)virtaddr >> 22; // Page table index
        uint32_t pi = ((uint32_t)virtaddr >> 12) & 0x3ff; // Page index
        if (!kernel_directory->tables[pti]) {
            create_page_table(kernel_directory, pti, 1, 1);
            kpe += sizeof(page_table_t);
        }
        kernel_directory->tables[pti]->pages[pi].present = 1; // Page is now present in memory
        kernel_directory->tables[pti]->pages[pi].rw = 1; // Page is writable
        kernel_directory->tables[pti]->pages[pi].user = 0; // Page is kernel mode
        kernel_directory->tables[pti]->pages[pi].frame_addr = (physaddr / 0x1000);
        set_frame(physaddr);
        physaddr += 0x1000; virtaddr += 0x1000; // Increment pointers
    }
    // Reset brk for kheap
    kbrk((void *)0xc0100000);
    // Load new page directory
    switch_page_directory(kernel_directory);
    (void)kvs; (void)kps; // Unused parameters
}

/* Load the main page directory, after kernel heap has been initialized.
 */
void fix_paging() {
    page_directory_t *dir = clone_page_directory(kernel_directory);
    switch_page_directory(dir);
}

/* Load a new page directory into the CR3 register.
 * @param page_directory        Address of the new page directory to load.
 */
void switch_page_directory(page_directory_t *page_directory) {
    current_directory = page_directory;
    asm volatile("mov %0, %%cr3" : : "r"(page_directory->physical_addr)); // Switch page directory
}

/* Page faults handler function.
 * @param r                     Registers.
 */
void page_fault_handler(registers_t *r) {
    physaddr_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r"(faulting_address));
    int present = !(r->err_code & 0x1);
    int rw = r->err_code & 0x2;
    int us = r->err_code & 0x4;
    int reserved = r->err_code & 0x8;
    int id = r->err_code & 0x10; (void)(id); // Unused parameter
    kprint("Page fault! ( ");
    if (present) kprint("present ");
    if (rw) kprint("read-only ");
    if (us) kprint("user-mode ");
    if (reserved) kprint("reserved ");
    kprint(") at 0x");
    char addr[9];
    itoa(faulting_address, addr, 16);
    kprint(addr);
    kprint("\n");
    panic("page fault");
    // TODO: must swap pages and load necessary ones!
    // NEEDED: disk driver!
}

/* Clone a page directory and the requires (only non-kernel ones) tables.
 * @param src           Source page directory.
 * @return              Pointer to the new page directory.
 */
page_directory_t *clone_page_directory(page_directory_t *src) {
    physaddr_t phys;
    page_directory_t *dir = (page_directory_t *)kcalloc_ap(sizeof(page_directory_t), &phys);
    dir->physical_addr = phys;
    uint32_t i;
    for (i = 0; i < 1024; ++i) { // For each page table
        if (!src->tables[i]) continue; // If it is empty, skip it
        if (kernel_directory->tables[i] == src->tables[i]) { // If it is the same as kernel dir, it should be linked
            dir->tables[i] = src->tables[i];
            dir->tables_physical[i] = src->tables_physical[i];
        } else { // Else, page table should be copied
            // Allocate page table
            page_table_t *tbl = (page_table_t *)kcalloc_ap(sizeof(page_table_t), &phys);
            // Save reference
            dir->tables[i] = tbl;
            // dir->tables_physical[i] = phys | 0x7; // User-mode, r/w, present
            dir->tables_physical[i] = phys | 0x3; // User-mode, r/w, present
            // Copy page table
            uint32_t j;
            for (j = 0; j < 1024; ++j) { // For each entry of the page tbale
                if (!src->tables[i]->pages[j].frame_addr) continue; // Skip empty pages
                alloc_frame(&tbl->pages[j], 0, 1); // Allocate a new frame, user, writable
                if (src->tables[i]->pages[j].present) tbl->pages[j].present = 1;
                if (src->tables[i]->pages[j].rw) tbl->pages[j].rw = 1;
                if (src->tables[i]->pages[j].user) tbl->pages[j].user = 1;
                if (src->tables[i]->pages[j].accessed) tbl->pages[j].accessed = 1;
                if (src->tables[i]->pages[j].dirty) tbl->pages[j].dirty = 1;
                // Temporarily map frame (in kernel virtual space, virtual addr 0xc3fff000) in order to copy it
                src->tables[0x30f]->pages[0x3ff].frame_addr = src->tables[i]->pages[j].frame_addr;
                src->tables[0x30f]->pages[0x3ff].present = 1;
                src->tables[0x30f]->pages[0x3ff].rw = 1;
                src->tables[0x30f]->pages[0x3ff].user = 1;
                src->tables[0x30f]->pages[0x3ff].accessed = 0;
                src->tables[0x30f]->pages[0x3ff].dirty = 0;
                asm volatile("invlpg (%0)" : : "r"(0xc3fff000) : "memory");
                // Copy physical frame
                uint32_t addr = (i << 22) | (j << 12);
                memcpy((void *)addr, (void *)0xc3fff000, 0x1000);
                // Remove temporary mapping and invalidate TLB
                src->tables[0x30f]->pages[0x3ff].frame_addr = 0;
                src->tables[0x30f]->pages[0x3ff].present = 0;
                src->tables[0x30f]->pages[0x3ff].rw = 0;
                src->tables[0x30f]->pages[0x3ff].user = 0;
                src->tables[0x30f]->pages[0x3ff].accessed = 0;
                src->tables[0x30f]->pages[0x3ff].dirty = 0;
                asm volatile("invlpg (%0)" : : "r"(0xc3fff000) : "memory");
            }
        }
    }
    return dir;
}

// Private functions

/* Create a new page table in a spacific page directory.
 * @param page_directory        Page directory.
 * @param index                 Page table index (i.e. PD entry number).
 * @param is_kernel             Set if page table is kernel, clear if user.
 * @param is_writable           Set if page table is writable, clear is read-only.
 */
void create_page_table(page_directory_t *page_directory, uint32_t index, uint8_t is_kernel, uint8_t is_writable) {
    physaddr_t phys;
    page_directory->tables[index] = (page_table_t *)dumb_kcalloc(sizeof(page_table_t), 1, &phys);
    uint32_t perm = 1; // Presence bit
    perm |= (is_writable? 1 : 0) << 1;
    perm |= (is_kernel? 0 : 1) << 2;
    page_directory->tables_physical[index] = phys | perm;
    page_directory->tables_physical[0] = phys | perm;
}

/* Set a bit in the frames bitset.
 * @param frame_address         Physical address of the frame.
 */
static void set_frame(physaddr_t frame_address) {
    uint32_t frame = frame_address / 0x1000; // Page table number (i.e. page directory entry)
    uint32_t index = INDEX(frame);
    uint32_t offset = OFFSET(frame);
    frames[index] |= (0x1 << offset);
}

/* Clear a bit in the frames bitset.
 * @param frame_address         Physical address of the frame.
 */
static void clear_frame(physaddr_t frame_address) {
    uint32_t frame = frame_address / 0x1000;
    uint32_t index = INDEX(frame);
    uint32_t offset = OFFSET(frame);
    frames[index] &= ~(0x1 << offset);
}

/* Test a bit in the frames bitset.
 * @param frame_address         Physical address of the frame.
 * @return                      Zero if frame is not set.
 */
// static uint32_t test_frame(physaddr_t frame_address) {
//     uint32_t frame = frame_address / 0x1000;
//     uint32_t index = INDEX(frame);
//     uint32_t offset = OFFSET(frame);
//     return (frames[index] & (0x1 << offset));
// }

/* Find the first free frame in memory. Very efficient implementation.
 * @return                      Index of the first free frame.
 */
static uint32_t first_free_frame() {
    uint32_t i, j;
    for (i = 0; i < INDEX(nframes); ++i) {
        if (frames[i] != 0xffffffff) {
            for (j = 0; j < 32; ++j) {
                uint32_t test = 0x1 << j;
                if (!(frames[i] & test)) return (uint32_t)(i*32+j);
            }
        }
    }
    return (uint32_t)-1;
}

/* Allocate a new frame.
 * @param page              Page to allocate in that frame.
 * @param is_kernel         Page is kernel-mode?
 * @param is_writable       Page is writable?
 */
void alloc_frame(page_t *page, int is_kernel, int is_writable) {
    if (page->frame_addr != 0) return; // Already allocated frame
    uint32_t index = first_free_frame();
    if (index == (uint32_t)-1) { // If there are no free frames
        panic("no free frames");
        // TODO: must implement frame replacement algorithm!
        // NEEDED: disk driver!
        return;
    }
    set_frame((physaddr_t)(index * 0x1000)); // Allocate frame
    page->present = 1;
    page->rw = ((is_writable)? 1 : 0);
    page->user = ((is_kernel)? 0 : 1);
    page->frame_addr = index;
}

/* Free an existing frame.
 * @param page              Page allocated in that frame.
 */
void free_frame(page_t *page) {
    uint32_t frame = page->frame_addr;
    if (!frame) return; // The frame is already free
    clear_frame((physaddr_t)(frame * 0x1000));
    page->frame_addr = 0;
}