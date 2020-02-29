// @desc     Memory-related functions header
// @author   Davide Della Giustina
// @date     07/01/2020

#ifndef MEM_H
#define MEM_H

#include <stddef.h>
#include <stdint.h>

typedef uint32_t physaddr_t; // Typedef for physical addresses (i.e. 32bit unsigned integers)

/* Copy a portion of memory from a source to a destination.
 * @param source        Source address.
 * @param destination   Destination address.
 * @param nbytes        Number of bytes to copy.
 */
void memcpy(void *source, void *destination, int nbytes);

/* Set a certain portion of memory to a value.
 * @param destination   Base memory address.
 * @param val           Value to set.
 * @param len           Length of memory portion (in bytes).
 */
void memset(void *destination, uint8_t val, size_t len);

#endif