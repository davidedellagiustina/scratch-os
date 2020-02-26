// @desc     Paging
// @author   Davide Della Giustina
// @date     16/02/2020

#include "paging.h"

uint32_t *frames; // Bitset of frames
uint32_t nframes;

physaddr_t boot_directory; // Backup of the boot directory
page_directory_t *kernel_directory, *current_directory;

#define INDEX(x)        (x / 32)
#define OFFSET(x)       (x % 32)

static inline void s(void *p) {
    __asm volatile("mov %0,%%cr3"::"r"(p));
}

// Private functions

static void set_frame(physaddr_t frame_address);
static void clear_frame(physaddr_t frame_address);
// static uint32_t test_frame(physaddr_t frame_address);
static uint32_t first_free_frame();
void alloc_frame(page_t *page, int is_kernel, int is_writable);
void free_frame(page_t *page);
void create_page_table(page_directory_t *page_directory, uint32_t index, uint8_t is_kernel, uint8_t is_writable);

// TEMP: Test function
// void test(void *kvs, void *kve, physaddr_t kps, physaddr_t kpe) {
//     (void)kvs; (void)kve; (void)kps; (void)kpe;
//     asm volatile("mov %%cr3, %0" : "=r"(boot_directory));
//     kbrk(kve);
//     char b[20];
//     itoa(boot_directory, b, 16);
//     kprint(b);
//     // asm volatile("mov %0, %%cr3" : : "r"(boot_directory));
//     // asm volatile("mov %0, %%eax" : : "r"(boot_directory));
//     // asm volatile("mov %%eax, %%cr3" : :);
//     // asm volatile("invlpg (%0)" : : "r"(768) : "memory");
//     // s((void *)boot_directory);
//     // asm volatile("l: jmp l");
// }

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
    // Create and fill new page directory (in kernel heap) and page tables
    kbrk(kve); // Set kernel heap start just after the kernel text segment
    uint32_t mem_size = TOTAL_RAM_SIZE * 0x100000; // RAM size (in MB) * 1MB (argument passed at compile time)
    nframes = mem_size / 0x1000;
    // frames = (uint32_t *)kcalloc(INDEX(nframes), 0, 0); // Allocate bitmap
    physaddr_t phys;
    // kernel_directory = (page_directory_t *)kcalloc(sizeof(page_directory_t), 1, &phys); // Allocate new page directory
    kernel_directory = (page_directory_t *)kmalloc(sizeof(page_directory_t), 1, &phys); // Allocate new page directory
    memset((uint8_t *)kernel_directory, 0, sizeof(page_directory_t));
    kernel_directory->physical_addr = phys;
    // Map kernel text segment
    // kvs = (void *)((uint32_t)kvs & 0xfffff000);
    // kps &= 0xfffff000;
    // kvs = (void *)0xc0000000;
    // kps = 0x0;
    // while (kps < kpe) {
    (void)kpe; (void)kvs; (void)kps; (void)kve;
    // while (kps < 4*0x100000) {
    //     uint32_t pti = (uint32_t)kvs >> 22; // Page table index
    //     uint32_t pi = ((uint32_t)kvs >> 12) & 0x3ff; // Page index
    //     if (!kernel_directory->tables[pti]) create_page_table(kernel_directory, pti, 1, 1);
    //     kernel_directory->tables[pti]->pages[pi].present = 1; // Page is now present in memory
    //     kernel_directory->tables[pti]->pages[pi].rw = 1; // Page is writable
    //     kernel_directory->tables[pti]->pages[pi].user = 0; // Page is kernel mode
    //     kernel_directory->tables[pti]->pages[pi].frame_addr = (kps / 0x1000);
    //     // set_frame(kps);
    //     kvs += 0x1000; kps += 0x1000; // Increment pointers
    // }

    kernel_directory->tables[768] = (page_table_t *)kmalloc(sizeof(page_table_t), 1, &phys);
    memset((uint8_t *)kernel_directory->tables[768], 0, sizeof(page_table_t));
    kernel_directory->tables_physical[768] = phys | 0x3; // Present, r/w, kernel
    int i;
    for (i = 0; i < 1024; ++i) {
        kernel_directory->tables[768]->pages[i].present = 1;
        kernel_directory->tables[768]->pages[i].rw = 1;
        kernel_directory->tables[768]->pages[i].frame_addr = i;
    }
    // uint32_t e = (uint32_t)ksbrk(0);
    // char buff[20];
    // itoa(e, buff, 16);
    // kprint("\n\nEnd of heap: "); kprint(buff); kprint("\n\n");
    // Page kernel heap
    // while (kps < 64 * 0x100000) { // 64MB
    //     uint32_t pti = (uint32_t)kvs >> 22; // Page table index
    //     uint32_t pi = ((uint32_t)kvs >> 12) & 0x3ff; // Page index
    //     if (!kernel_directory->tables[pti]) create_page_table(kernel_directory, pti, 1, 1);
    //     kernel_directory->tables[pti]->pages[pi].present = 1; // Page is now present in memory
    //     kernel_directory->tables[pti]->pages[pi].rw = 1; // Page is writable
    //     kernel_directory->tables[pti]->pages[pi].user = 0; // Page is kernel mode
    //     kernel_directory->tables[pti]->pages[pi].frame_addr = (kps / 0x1000);
    //     set_frame(kps);
    //     kvs += 0x1000; kps += 0x1000; // Increment pointers
    // }
    // Register page fault handler and switch page directory
    register_interrupt_handler(14, page_fault_handler);
    // uint32_t end = ((page_table_t *)(kernel_directory->tables_physical[768] + 0xc0000000 - 3))->pages[0].frame_addr;
    // uint32_t end = kernel_directory->tables[768]->pages[0].frame_addr;
    uint32_t end = (uint32_t)setup_paging;
    char buf[20];
    itoa(end, buf, 16);
    kprint("\n\nEnd of heap: "); kprint(buf);
    // asm volatile("l: jmp l");
    switch_page_directory(kernel_directory);
    (void)kve; // Unused
}

/* Load a new page directory into the CR3 register.
 * @param page_directory        Address of the new page directory to load.
 */
void switch_page_directory(page_directory_t *page_directory) {
    current_directory = page_directory;
    asm volatile("mov %0, %%cr3" : : "r"(page_directory->tables_physical));
    // Since paging was enabled back before the kernel was loaded, we do not need to set the PG bit in cr0 here
}

// /* Retrieve the pointer to the required page.
//  * @param address               Virtual address of the required page.
//  * @param make                  If set, the page table should be created if it is not found.
//  * @param page_directory        Page directory.
//  * @return                      Pointer to the required page.
//  */
// page_t *get_page(void *address, int make, page_directory_t *page_directory) {
//     address /= 0x1000; // Address to index
//     uint32_t table_index = address / 1024;
//     if (page_directory->tables[table_index]) {
//         return &page_directory->tables[table_index]->pages[address%1024];
//     } else if (make) {
//         uint32_t phys;
//         page_directory->tables[table_index] = (page_table_t *)kcalloc(sizeof(page_table_t), 1, &phys);
//         page_directory->tables_physical[table_index] = phys | 0x3; // Present, r/w, kernel
//         int i;
//         for (i = 0; i < 1024; ++i) {
//             page_directory->tables[table_index]->pages[i].present = 1;
//             page_directory->tables[table_index]->pages[i].rw = 1;
//             page_directory->tables[table_index]->pages[i].frame_addr = i;
//         }
//         return &page_directory->tables[table_index]->pages[address%1024];
//     } else return (page_t *)0;
// }

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

// Private functions

/* Create a new page table in a spacific page directory.
 * @param page_directory        Page directory.
 * @param index                 Page table index (i.e. PD entry number).
 * @param is_kernel             Set if page table is kernel, clear if user.
 * @param is_writable           Set if page table is writable, clear is read-only.
 */
void create_page_table(page_directory_t *page_directory, uint32_t index, uint8_t is_kernel, uint8_t is_writable) {
    physaddr_t phys;
    page_directory->tables[index] = (page_table_t *)kcalloc(sizeof(page_table_t), 1, &phys);
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