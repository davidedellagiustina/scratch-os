// @desc     Memory-related functions
// @author   Davide Della Giustina
// @date     07/01/2020

#include "mem.h"

/* Copy a portion of memory from a source to a destination.
 * @param source        Source address.
 * @param destination   Destination address.
 * @param nbytes        Number of bytes to copy.
 */
void memcpy(void *source, void *destination, int nbytes) {
    uint8_t *src = (uint8_t *)source, *dst = (uint8_t *)destination;
    int i;
    for (i = 0; i < nbytes; ++i) {
        *(dst + i) = *(src + i);
    }
}

/* Set a certain portion of memory to a value.
 * @param destination   Base memory address.
 * @param val           Value to set.
 * @param len           Length of memory portion (in bytes).
 */
void memset(void *destination, uint8_t val, size_t len) {
    uint8_t *temp = (uint8_t *)destination;
    for (; len != 0; --len) *temp++ = val;
}