// @desc     Paging
// @author   Davide Della Giustina
// @date     11/01/2020

#include "paging.h"

uint32_t *frames; // Bitset of frames
uint32_t nframes;

extern uint32_t sys_brk; // Declared in 'mem.c'
page_directory_t *kernel_directory, *current_directory;

#define INDEX(x)        (x / 32)
#define OFFSET(x)       (x % 32)

// Private functions

static void set_frame(uint32_t frame_address);
static void clear_frame(uint32_t frame_address);
// static uint32_t test_frame(uint32_t frame_address);
static uint32_t first_free_frame();
void alloc_frame(page_t *page, int is_kernel, int is_writable);
void free_frame(page_t *page);

// Public functions

/* Initialize paging environment.
 */
void init_paging() {
    uint32_t mem_end_page = 0x1000000; // 16MB of physical memory
    nframes = mem_end_page / 0x1000;
    //frames = (uint32_t *)kmalloc(INDEX(nframes), 0, 0);
    frames = (uint32_t *)kmalloc(INDEX(nframes), 0, NULL);
    memset((uint8_t *)frames, 0, INDEX(nframes));
    //uint32_t physical;
    //kernel_directory = (page_directory_t *)kmalloc(sizeof(page_directory_t), 1, &physical);
    kernel_directory = (page_directory_t *)kmalloc(sizeof(page_directory_t), 1, NULL);
    memset((uint8_t *)kernel_directory, 0, sizeof(page_directory_t));
    current_directory = kernel_directory;
    unsigned int i = 0;
    while (i < sys_brk) {
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }
    register_interrupt_handler(14, page_fault_handler);
    load_page_directory(kernel_directory);
}

/* Load a new page directory into the CR3 register.
 * @param page_directory        Address of the new page directory to load.
 */
void load_page_directory(page_directory_t *page_directory) {
    current_directory = page_directory;
    asm volatile("mov %0, %%cr3" : : "r"(&page_directory->tables_physical));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // Set paging bit
    //
    uint32_t dir;
    for (int i = 0; i < 3; ++i) {
        asm volatile("mov [%%cr3], %0" : "=r"(cr3));
    }
    char tmp[33];
    itoa(cr3, tmp, 16);
    kprint("cr3: 0x");
    kprint(tmp);
    kprint("\n");
    asm volatile("hlt");
    return;
    //
    asm volatile("mov %0, %%cr0" : : "r"(cr0));
}

/* Retrieve the pointer to the required page.
 * @param address               Address of the required page.
 * @param make                  If set, the page table should be created if it is not found.
 * @param page_directory        Page directory.
 * @return                      Pointer to the required page.
 */
page_t *get_page(uint32_t address, int make, page_directory_t *page_directory) {
    address /= 0x1000; // Address to index
    uint32_t table_index = address / 1024;
    if (page_directory->tables[table_index]) {
        return &page_directory->tables[table_index]->pages[address%1024];
    } else if (make) {
        uint32_t tmp;
        page_directory->tables[table_index] = (page_table_t *)kmalloc(sizeof(page_table_t), 1, &tmp);
        memset((uint8_t *)page_directory->tables[table_index], 0, 0x1000);
        page_directory->tables_physical[table_index] = tmp | 0x7; // Present, r/w, user
        return &page_directory->tables[table_index]->pages[address%1024];
    } else return 0;
}

/* Page faults handler function.
 * @param r                     Registers.
 */
void page_fault_handler(registers_t *r) {
    uint32_t faulting_address;
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
    kprint("*** System panic: page fault ***\n");
}

// Private functions

/* Set a bit in the frames bitset.
 * @param frame_address         Address of the frame.
 */
static void set_frame(uint32_t frame_address) {
    uint32_t frame = frame_address / 0x1000;
    uint32_t index = INDEX(frame);
    uint32_t offset = OFFSET(frame);
    frames[index] |= (0x1 << offset);
}

/* Clear a bit in the frames bitset.
 * @param frame_address         Address of the frame.
 */
static void clear_frame(uint32_t frame_address) {
    uint32_t frame = frame_address / 0x1000;
    uint32_t index = INDEX(frame);
    uint32_t offset = OFFSET(frame);
    frames[index] &= ~(0x1 << offset);
}

/* Test a bit in the frames bitset.
 * @param frame_address         Address of the frame.
 * @return                      Zero if frame is not set.
 */
/*static uint32_t test_frame(uint32_t frame_address) {
    uint32_t frame = frame_address / 0x1000;
    uint32_t index = INDEX(frame);
    uint32_t offset = OFFSET(frame);
    return (frames[index] & (0x1 << offset));
}*/

/* Find the first free frame in memory. Very efficient implementation.
 * @return                      Address of the first free frame.
 */
static uint32_t first_free_frame() {
    uint32_t i, j;
    for (i = 0; i < INDEX(nframes); ++i) {
        if (frames[i] != 0xffffffff) {
            for (j = 0; j < 32; ++j) {
                uint32_t test = 0x1 << j;
                if (!(frames[i] & test)) return (i*32+j);
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
        kprint("*** System panic: no free frames ***\n");
        asm volatile("hlt");
        return;
    }
    set_frame(index * 0x1000);
    page->present = 1;
    page->rw = ((is_writable)? 1 : 0);
    page->user = ((is_kernel)? 0 : 1);
    page->frame_addr = index;
}

/* Free an existing frame.
 * @param page              Page allocated in that frame.
 */
void free_frame(page_t *page) {
    uint32_t frame;
    if (!(frame = page->frame_addr)) return; // The frame is already free
    clear_frame(frame);
    page->frame_addr = 0x0;
}