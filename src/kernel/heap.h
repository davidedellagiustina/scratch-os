// @desc     Kernel heap header
// @author   Davide Della Giustina
// @date     25/02/2020

#ifndef HEAP_H
#define HEAP_H

#include "../libc/mem.h"

/* Set the kernel data segment limit to a certain address.
 * @param brk       New kernel data segment limit.
 */
void kbrk(void *brk);

/* Increment the kernel data segment limit by a certain amount.
 * @param incr      Number of bytes to add to the kernel data segment.
 * @return          Pointer to the free memory allocated.
 */
void *ksbrk(size_t incr);

/* Memory allocator.
 * @param size          Size of the memory chunk needed.
 * @param align         Pages should be aligned (4K)?
 * @param physical      Physical base address of the allocated memory (buffer).
 * @return              Pointer to allocated memory.
 */
void *kmalloc(size_t size, int align, physaddr_t *physical);

/* Memory allocator and initializator.
 * @param size          Size of the memory chunk needed.
 * @param align         Pages should be aligned (4K)?
 * @param physical      Physical base address of the allocated memory (buffer).
 * @return              Pointer to allocated memory.
 */
void *kcalloc(size_t size, int align, physaddr_t *physical);

#endif