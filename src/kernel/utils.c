// @desc     Kernel useful functions.
// @author   Davide Della Giustina
// @date     08/12/2019

#include "utils.h"

/* Copy a portion of memory from a source to a destination.
 * @param source        Source address.
 * @param destination   Destination address.
 * @param nr_bytes      Number of bytes to copy.
 */
inline void memory_copy(unsigned char *source, unsigned char *destination, uint32_t nr_bytes) {
    uint32_t i;
    for (i = 0; i < nr_bytes; ++i) {
        *(destination + i) = *(source + i);
    }
}