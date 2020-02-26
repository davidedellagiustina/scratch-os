// @desc     Kernel heap header
// @author   Davide Della Giustina
// @date     25/02/2020

#include "heap.h"

void *kernel_brk; // Must be set before using kernel heap

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

/* Memory allocator.
 * @param size          Size of the memory chunk needed.
 * @param align         Pages should be aligned (4K)?
 * @param physical      Physical base address of the allocated memory (buffer).
 * @return              Pointer to allocated memory.
 */
void *kmalloc(size_t size, int align, physaddr_t *physical) {
    uint32_t brk = (uint32_t)ksbrk(0); // Get current brk
    if (align && (brk & 0xfff)) {
        brk &= 0xfffff000;
        brk += 0x1000; // 4K alignment
        kbrk((void *)brk);
    }
    if (physical) *physical = (physaddr_t)kernel_brk - 0xc0000000;
    return ksbrk(size);
}

/* Memory allocator and initializator.
 * @param size          Size of the memory chunk needed.
 * @param align         Pages should be aligned (4K)?
 * @param physical      Physical base address of the allocated memory (buffer).
 * @return              Pointer to allocated memory.
 */
void *kcalloc(size_t size, int align, physaddr_t *physical) {
    void *ptr = kmalloc(size, align, physical); // Allocate memory
    memset((uint8_t *)ptr, 0, size); // Initialize to zero
    return ptr;
}