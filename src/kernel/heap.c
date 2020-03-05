// @desc     Kernel heap header
// @author   Davide Della Giustina
// @date     25/02/2020

#include "heap.h"

void *kernel_brk = (void *)0xc0100000; // Virtual, aligned, after end of VGA/ROM memory (0xa0000 - 0xfffff)
heap_t *kernel_heap; // Kernel heap

/* Set the kernel data segment limit to a certain address.
 * @param brk       New kernel data segment limit.
 */
void kbrk(void *brk) {
    kernel_brk = brk;
}

/* Increment the kernel data segment limit by a certain amount.
 * @param incr      Number of bytes to add to the kernel data segment.
 * @return          Pointer to the free memory allocated.
 */
void *ksbrk(size_t incr) {
    void *ptr = kernel_brk;
    kernel_brk += incr;
    return ptr;
}

/* Dumb memory allocator (used for initializing paging).
 * @param size          Size of the memory chunk needed.
 * @param align         Pages should be aligned (4K)?
 * @param physical      Physical base address of the allocated memory (buffer).
 * @return              Pointer to allocated memory.
 */
void *dumb_kmalloc(size_t size, int align, physaddr_t *physical) {
    uint32_t brk = (uint32_t)ksbrk(0); // Get current brk
    if (align && (brk & 0xfff)) {
        brk &= 0xfffff000;
        brk += 0x1000; // 4K alignment
        kbrk((void *)brk);
    }
    if (physical) *physical = (physaddr_t)kernel_brk - 0xc0000000;
    return ksbrk(size);
}

/* Dumb memory allocator and initializator (used for initializing paging).
 * @param size          Size of the memory chunk needed.
 * @param align         Pages should be aligned (4K)?
 * @param physical      Physical base address of the allocated memory (buffer).
 * @return              Pointer to allocated memory.
 */
void *dumb_kcalloc(size_t size, int align, physaddr_t *physical) {
    void *ptr = dumb_kmalloc(size, align, physical); // Allocate memory
    memset(ptr, 0, size); // Initialize to zero
    return ptr;
}

/* Find the smallest hole in a heap, basing on the index.
 * @param heap          Heap.
 * @param size          Size we are looking for.
 * @param page_align    The hole should be page aligned?
 * @return              Index of the found hole, -1 if not found.
 */
static int32_t find_smallest_hole(heap_t *heap, uint32_t size, uint8_t page_align) {
    uint32_t iterator = 0;
    while (iterator < heap->index.size) {
        heap_header_t *header = (heap_header_t *)get_ordered_array(&heap->index, iterator);
        if (page_align > 0) {
            uint32_t location = (uint32_t)header;
            int32_t offset = 0;
            if (((location + sizeof(heap_header_t)) & 0xfff) != 0) {
                offset = 0x1000 - (location + sizeof(heap_header_t)) % 0x1000;
            }
            int32_t hole_size = (int32_t)header->size - offset;
            if (hole_size >= (int32_t)size) break;
        } else if (header->size >= size) break;
        ++iterator;
    }
    if (iterator == heap->index.size) return -1;
    else return iterator;
}

/* Comparing function for ordering heap blocks (increasing size).
 * @param a             First block to compare.
 * @param b             Second block to compare.
 * @return              1 if a < b, 0 otherwise.
 */
static int8_t heap_header_t_less_than(void *a, void *b) {
    return ((((heap_header_t *)a)->size < ((heap_header_t *)b)->size)? 1 : 0);
}

/* Create a new heap structure.
 * @param start         Address where to place the ordered array (must be page aligned).
 * @param end           End of the allocated memory for the heap.
 * @param max           Maximum address the heap can reach.
 * @param supervisor    The requested pages will be in kernel (supervisor) mode?
 * @param readonly      The requested pages will be read-only?
 * @return              Pointer to the heap structure.
 */
heap_t *create_heap(void *start, void *end, void *max, uint8_t supervisor, uint8_t readonly) {
    kbrk((void *)0xc0050000);
    heap_t *heap = (heap_t *)dumb_kmalloc(sizeof(heap_t), 0, 0); // Heap struct at address 0xc00c0000 + index size
    kbrk((void *)0xc0100000);
    assert((uint32_t)start % 0x1000 == 0); // Must be page-aligned
    assert((uint32_t)end % 0x1000 == 0);
    heap->index = place_ordered_array(start, KHEAP_INDEX_SIZE, &heap_header_t_less_than); // Initialize index of holes
    start += sizeof(void *) * KHEAP_INDEX_SIZE; // Skip index
    uint32_t s = (uint32_t)start;
    if ((s & 0xfff) != 0) { // Check alignment
        s &= 0xfffff000;
        s += 0x1000;
    }
    start = (void *)s; // Update start ptr
    // Populate heap structure
    heap->start_addr = start;
    heap->end_addr = end;
    heap->max_addr = max;
    heap->supervisor = supervisor;
    heap->readonly = readonly;
    // Start with one large hole (in the index)
    heap_header_t *hole = (heap_header_t *)start;
    hole->size = end - start;
    hole->magic = HEAP_MAGIC;
    hole->is_hole = 1;
    heap_footer_t *hole_footer = (heap_footer_t *)((uint32_t)hole + hole->size - sizeof(heap_footer_t));
    hole_footer->magic = HEAP_MAGIC;
    hole_footer->header = hole;
    insert_ordered_array(&heap->index, (void *)hole);
    return heap; // Return heap structure
}

/* Expand the heap to a new size.
 * @param heap          Heap.
 * @param new_size      New size of the heap.
 */
static void expand(heap_t *heap, uint32_t new_size) {
    assert(new_size > (uint32_t)(heap->end_addr - heap->start_addr));
    if ((new_size & 0xfff) != 0) {
        new_size &= 0xfffff000;
        new_size += 0x1000;
    }
    assert((uint32_t)heap->start_addr + new_size <= (uint32_t)heap->max_addr);
    heap->end_addr = heap->start_addr + new_size;
}

/* Contract the heap to a new size.
 * @param heap          Heap.
 * @param new_size      New size of the heap.
 * @return              New size of the heap (page-aligned);
 */
static uint32_t contract(heap_t *heap, uint32_t new_size) {
    assert(new_size < (uint32_t)(heap->end_addr - heap->start_addr));
    if (new_size & 0xfff) {
        new_size &= 0xfffff000;
        new_size += 0x1000;
    }
    if (new_size < KHEAP_MIN_SIZE) new_size = KHEAP_MIN_SIZE;
    heap->end_addr = heap->start_addr + new_size;
    return new_size;
}

#include "../drivers/vga.h"

/* Allocate a new block in the heap.
 * @param heap          Heap.
 * @param size          Size of the needed block.
 * @param page_align    Should the block be page-aligned?
 * @return              Pointer to the newly allocated block.
 */
void *alloc(heap_t *heap, uint32_t size, uint8_t page_align) {
    uint32_t new_size = size + sizeof(heap_header_t) + sizeof(heap_footer_t); // Size of needed block is size + header + footer
    int32_t iterator = find_smallest_hole(heap, new_size, page_align);
    if (iterator == -1) { // If  no hole has been found
        uint32_t old_length = (uint32_t)heap->end_addr - (uint32_t)heap->start_addr;
        uint32_t old_end_addr = (uint32_t)heap->end_addr;
        expand(heap, old_length + new_size);
        uint32_t new_length = (uint32_t)heap->end_addr - (uint32_t)heap->start_addr;
        iterator = 0;
        int32_t idx = -1;
        uint32_t value = 0x0;
        while ((uint32_t)iterator < heap->index.size) {
            uint32_t tmp = (uint32_t)get_ordered_array(&heap->index, iterator);
            if (tmp > value) {
                value = tmp;
                idx = iterator;
            }
            ++iterator;
        }
        if (idx == -1) {
            heap_header_t *header = (heap_header_t *)old_end_addr;
            header->magic = HEAP_MAGIC;
            header->size = new_length - old_length;
            header->is_hole = 1;
            heap_footer_t *footer = (heap_footer_t *)(old_end_addr + header->size - sizeof(heap_footer_t));
            footer->magic = HEAP_MAGIC;
            footer->header = header;
            insert_ordered_array(&heap->index, (void *)header);
        } else {
            heap_header_t *header = (heap_header_t *)get_ordered_array(&heap->index, idx);
            header->size += new_length - old_length;
            heap_footer_t *footer = (heap_footer_t *)((uint32_t)header + header->size - sizeof(heap_footer_t));
            footer->header = header;
            footer->magic = HEAP_MAGIC;
        }
        return alloc(heap, size, page_align);
    }
    heap_header_t *orig_hole_header = (heap_header_t *)get_ordered_array(&heap->index, iterator);
    uint32_t orig_hole_pos = (uint32_t)orig_hole_header;
    uint32_t orig_hole_size = orig_hole_header->size;
    // Should we split hole into two parts?
    if (orig_hole_size - new_size < sizeof(heap_header_t) + sizeof(heap_footer_t)) { // If not, increase size we are going to allocate
        size += orig_hole_size - new_size;
        new_size = orig_hole_size;
    }
    // Check page alignment
    if (page_align && ((orig_hole_pos + sizeof(heap_header_t)) & 0xfff)) {
        uint32_t new_location = orig_hole_pos + 0x1000 - (orig_hole_pos & 0xfff) - sizeof(heap_header_t);
        heap_header_t *hole_header = (heap_header_t *)orig_hole_pos;
        hole_header->size = 0x1000 - (orig_hole_pos & 0xfff) - sizeof(heap_header_t);
        hole_header->magic = HEAP_MAGIC;
        hole_header->is_hole = 1;
        heap_footer_t *hole_footer = (heap_footer_t *)((uint32_t)new_location - sizeof(heap_footer_t));
        hole_footer->magic = HEAP_MAGIC;
        hole_footer->header = hole_header;
        orig_hole_pos = new_location;
        orig_hole_size = orig_hole_size - hole_header->size;
    } else remove_ordered_array(&heap->index, iterator);
    // Overwrite original header
    heap_header_t *block_header = (heap_header_t *)orig_hole_pos;
    block_header->magic = HEAP_MAGIC;
    block_header->is_hole = 0;
    block_header->size = new_size;
    // And footer
    heap_footer_t *block_footer = (heap_footer_t *)(orig_hole_pos + sizeof(heap_header_t) + size);
    block_footer->magic = HEAP_MAGIC;
    block_footer->header = block_header;
    // Maybe new hole after allocated block
    if (orig_hole_size - new_size > 0) {
        heap_header_t *hole_header = (heap_header_t *)(orig_hole_pos + sizeof(heap_header_t) + size + sizeof(heap_footer_t));
        hole_header->magic = HEAP_MAGIC;
        hole_header->is_hole = 1;
        hole_header->size = orig_hole_size - new_size;
        heap_footer_t *hole_footer = (heap_footer_t *)((uint32_t)hole_header + orig_hole_size - new_size -sizeof(heap_footer_t));
        if ((uint32_t)hole_footer < (uint32_t)heap->end_addr) {
            hole_footer->magic = HEAP_MAGIC;
            hole_footer->header = hole_header;
        }
        insert_ordered_array(&heap->index, (void *)hole_header);
    }
    return (void *)((uint32_t)block_header + sizeof(heap_header_t));
}

/* Free some allocated space.
 * @param heap          Heap.
 * @param p             Pointer to allocated area.
 */
void free(heap_t *heap, void *p) {
    if (p == 0) return; // Null pointers
    heap_header_t *header = (heap_header_t *)((uint32_t)p - sizeof(heap_header_t));
    heap_footer_t *footer = (heap_footer_t *)((uint32_t)header + header->size - sizeof(heap_footer_t));
    assert(header->magic == HEAP_MAGIC);
    assert(footer->magic == HEAP_MAGIC);
    header->is_hole = 1;
    uint8_t do_add = 1;
    // Unify left
    heap_footer_t *test_footer = (heap_footer_t *)((uint32_t)header - sizeof(heap_footer_t));
    if (test_footer->magic == HEAP_MAGIC &&test_footer->header->is_hole == 1) {
        uint32_t cache_size = header->size;
        header = test_footer->header;
        footer->header = header;
        header->size += cache_size;
        do_add = 0;
    }
    // Unify right
    heap_header_t *test_header = (heap_header_t *)((uint32_t)footer + sizeof(heap_footer_t));
    if (test_header->magic == HEAP_MAGIC && test_header->is_hole) {
        header->size += test_header->size;
        test_footer = (heap_footer_t *)((uint32_t)test_header + test_header->size - sizeof(heap_footer_t));
        footer = test_footer;
        uint32_t iterator = 0;
        while ((iterator < heap->index.size) && (get_ordered_array(&heap->index, iterator) != (void *)test_header)) ++iterator;
        assert(iterator < heap->index.size);
        remove_ordered_array(&heap->index, iterator);
    }
    // If freed area is at the end of the heap, contract it
    if ((uint32_t)footer + sizeof(heap_footer_t) == (uint32_t)heap->end_addr) {
        uint32_t old_length = (uint32_t)heap->end_addr - (uint32_t)heap->start_addr;
        uint32_t new_length = contract(heap, (uint32_t)header - (uint32_t)heap->start_addr);
        if (header->size - (old_length - new_length) > 0) {
            header->size -= old_length - new_length;
            footer = (heap_footer_t *)((uint32_t)header + header->size - sizeof(heap_footer_t));
            footer->magic = HEAP_MAGIC;
            footer->header = header;
        } else {
            uint32_t iterator = 0;
            while ((iterator < heap->index.size) && (get_ordered_array(&heap->index, iterator) != (void *)test_header)) ++iterator;
            if (iterator < heap->index.size) remove_ordered_array(&heap->index, iterator);
        }
    }
    if (do_add) insert_ordered_array(&heap->index, (void *)header);
}

/* Initialize kernel heap.
 */
void kheap_init() {
    void *start = ksbrk(0);
    kernel_heap = create_heap(start, start + KHEAP_INITIAL_SIZE, start + KHEAP_MAX_SIZE, 1, 0);
}

/* Allocate space in the kernel heap.
 * @param size              Size of requested space.
 * @return                  Pointer to newly allocated area.
 */
void *kmalloc(uint32_t size) {
    return alloc(kernel_heap, size, 0);
}

/* Allocate space in the kernel heap.
 * @param size              Size of requested space.
 * @param phys              Where physical address will be stored.
 * @return                  Pointer to newly allocated area.
 */
void *kmalloc_ap(uint32_t size, physaddr_t *phys) {
    void *allocated = alloc(kernel_heap, size, 1);
    *phys = (physaddr_t)allocated - 0xc0000000;
    return allocated;
}

/* Allocate space in the kernel heap, then initialize it to 0.
 * @param size              Size of requested space.
 * @return                  Pointer to newly allocated area.
 */
void *kcalloc(uint32_t size) {
    void *allocated = alloc(kernel_heap, size, 0);
    memset(allocated, 0, size);
    return allocated;
}

/* Allocate space in the kernel heap, then initialize it to 0.
 * @param size              Size of requested space.
 * @param phys              Where physical address will be stored.
 * @return                  Pointer to newly allocated area.
 */
void *kcalloc_ap(uint32_t size, physaddr_t *phys) {
    void *allocated = alloc(kernel_heap, size, 1);
    memset(allocated, 0, size);
    *phys = (physaddr_t)allocated - 0xc0000000;
    return allocated;
}

/* Expand a previously allocated area to a new size.
 * @param p                 Pointer to already allocated area.
 * @param size              New size.
 * @return                  Pointer to the enlarged area.
 */
void *krealloc(void *p, uint32_t size) {
    heap_header_t *header = (heap_header_t *)((uint32_t)p - sizeof(heap_header_t));
    uint32_t old_size = header->size - sizeof(heap_header_t) - sizeof(heap_footer_t);
    void *allocated = alloc(kernel_heap, size, 0);
    memcpy(p, allocated, old_size);
    kfree(p);
    return allocated;
}

/* Free some space in the kernel heap.
 * @param p                 Pointer to allocated space.
 */
void kfree(void *p) {
    free(kernel_heap, p);
}