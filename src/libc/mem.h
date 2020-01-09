// @desc     Memory-related functions header
// @author   Davide Della Giustina
// @date     07/01/2020

#ifndef MEM_H
#define MEM_H

#include <stddef.h>
#include <stdint.h>

/* Copy a portion of memory from a source to a destination.
 * @param src           Source address.
 * @param dst           Destination address.
 * @param nbytes        Number of bytes to copy.
 */
void memcpy(uint8_t *src, uint8_t *dst, int nbytes);

/* Set a certain portion of memory to a value.
 * @param dst           Base memory address.
 * @param val           Value to set.
 * @param len           Length of memory portion.
 */
void memset(uint8_t *dst, uint8_t val, size_t len);

/* Memory allocator.
 * @param size          Size of the memory chunk needed.
 * @param align         Pages should be aligned (4K)?
 * @param physical      Physical base address of the allocated memory (buffer).
 */
uint32_t kmalloc(size_t size, int align, uint32_t *physical);

#endif