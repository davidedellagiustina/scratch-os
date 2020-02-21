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
 * @param len           Length of memory portion (in bytes).
 */
void memset(uint8_t *dst, uint8_t val, size_t len) {
    uint8_t *temp = (uint8_t *)dst;
    for (; len != 0; --len) *temp++ = val;
}

physical_address_t sys_brk = 0x10000;

/* Memory allocator.
 * @param size          Size of the memory chunk needed.
 * @param align         Pages should be aligned (4K)?
 * @param physical      Physical base address of the allocated memory (buffer).
 * @return              Pointer to allocated memory.
 */
void *kmalloc(size_t size, int align, physical_address_t *physical) {
    if (align == 1 && (sys_brk & 0xfffff000)) {
        sys_brk &= 0xfffff000;
        sys_brk += 0x1000; // 4K
    }
    if (physical) *physical = sys_brk;
    uint32_t r = sys_brk;
    sys_brk += size;
    return (void *)r;
}