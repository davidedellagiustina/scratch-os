// @desc     Memory-related functions
// @author   Davide Della Giustina
// @date     07/01/2020

#include "mem.h"

/* Copy a portion of memory from a source to a destination.
 * @param src           Source address.
 * @param dst           Destination address.
 * @param nbytes        Number of bytes to copy.
 */
void memcpy(uint8_t *src, uint8_t *dst, int nbytes) {
    int i;
    for (i = 0; i < nbytes; ++i) {
        *(dst + i) = *(src + i);
    }
}

/* Set a certain portion of memory to a value.
 * @param dst           Base memory address.
 * @param val           Value to set.
 * @param len           Length of memory portion.
 */
void memset(uint8_t *dst, uint8_t val, size_t len) {
    uint8_t *temp = (uint8_t *)dst;
    for (; len != 0; --len) *temp++ = val;
}

uint32_t free_mem_addr = 0x10000;

/* Memory allocator.
 * @param size          Size of the memory chunk needed.
 * @param align         Pages should be aligned (4K)?
 * @param physical_addr Physical base address of the allocated memory (buffer).
 */
uint32_t kmalloc(size_t size, int align, uint32_t *physical_addr) {
    if (align == 1 && (free_mem_addr & 0xfffff000)) {
        free_mem_addr &= 0xfffff000;
        free_mem_addr += 0x1000; // 4K
    }
    if (physical_addr) *physical_addr = free_mem_addr;
    uint32_t ret = free_mem_addr;
    free_mem_addr += size;
    return ret;
}