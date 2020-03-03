// @desc     Kernel heap header
// @author   Davide Della Giustina
// @date     25/02/2020

#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include "../data_structures/ordered_array.h"
#include "../libc/assert.h"
#include "../libc/mem.h"

#define HEAP_MAGIC          0xdeadc0de // Magic number for the heap
#define KHEAP_INITIAL_SIZE  0x100000   // Initial size of the kernel heap
#define KHEAP_MIN_SIZE      0x70000    // Minimum size for the kernel heap
#define KHEAP_MAX_SIZE      0x3500000  // Maximum size for the kernel heap
#define KHEAP_INDEX_SIZE    0x20000    // Index size for the kernel heap

// Heap header
typedef struct {
    uint32_t magic; // Magic number
    uint8_t is_hole; // Set if this block is a hole
    uint32_t size; // Size of the block, including footer
} heap_header_t;

// Heap footer
typedef struct {
    uint32_t magic; // Magic number
    heap_header_t *header; // Pointer to block header
} heap_footer_t;

// Heap
typedef struct {
    ordered_array_t index; // Heap blocks
    void *start_addr;
    void *end_addr;
    void *max_addr;
    uint8_t supervisor;
    uint8_t readonly;
} heap_t;

/* Set the kernel data segment limit to a certain address.
 * @param brk       New kernel data segment limit.
 */
void kbrk(void *brk);

/* Increment the kernel data segment limit by a certain amount.
 * @param incr      Number of bytes to add to the kernel data segment.
 * @return          Pointer to the free memory allocated.
 */
void *ksbrk(size_t incr);

/* Dumb memory allocator (used for initializing paging).
 * @param size          Size of the memory chunk needed.
 * @param align         Pages should be aligned (4K)?
 * @param physical      Physical base address of the allocated memory (buffer).
 * @return              Pointer to allocated memory.
 */
void *dumb_kmalloc(size_t size, int align, physaddr_t *physical);

/* Dumb memory allocator and initializator (used for initializing paging).
 * @param size          Size of the memory chunk needed.
 * @param align         Pages should be aligned (4K)?
 * @param physical      Physical base address of the allocated memory (buffer).
 * @return              Pointer to allocated memory.
 */
void *dumb_kcalloc(size_t size, int align, physaddr_t *physical);

/* Create a new heap structure.
 * @param start         Address where to place the ordered array (must be page aligned).
 * @param end           End of the allocated memory for the heap.
 * @param max           Maximum address the heap can reach.
 * @param supervisor    The requested pages will be in kernel (supervisor) mode?
 * @param readonly      The requested pges will be read-only?
 * @return              Pointer to the heap structure.
 */
heap_t *create_heap(void *start, void *end, void *max, uint8_t supervisor, uint8_t readonly);

/* Allocate a new block in the heap.
 * @param heap          Heap.
 * @param size          Size of the needed block.
 * @param page_align    Should the block be page-aligned?
 * @return              Pointer to the newly allocated block.
 */
void *alloc(heap_t *heap, uint32_t size, uint8_t page_align);

/* Free some allocated space.
 * @param heap          Heap.
 * @param p             Pointer to allocated area.
 */
void free(heap_t *heap, void *p);

/* Initialize kernel heap.
 */
void kheap_init();

/* Allocate space in the kernel heap.
 * @param size              Size of requested space.
 * @return                  Pointer to newly allocated area.
 */
void *kmalloc(uint32_t size);

/* Allocate space in the kernel heap, then initialize it to 0.
 * @param size              Size of requested space.
 * @return                  Pointer to newly allocated area.
 */
void *kcalloc(uint32_t size);

/* Expand a previously allocated area to a new size.
 * @param p                 Pointer to already allocated area.
 * @param size              New size.
 * @return                  Pointer to the enlarged area.
 */
void *krealloc(void *p, uint32_t size);

/* Free some space in the kernel heap.
 * @param p                 Pointer to allocated space.
 */
void kfree(void *p);

#endif