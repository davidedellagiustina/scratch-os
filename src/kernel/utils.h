// @desc     Kernel useful functions header.
// @author   Davide Della Giustina
// @date     08/12/2019

#ifndef UTILS_H
#define UTILS_H

#include "../cpu/types.h"

/* Copy a portion of memory from a source to a destination.
 * @param source        Source address.
 * @param destination   Destination address.
 * @param nr_bytes      Number of bytes to copy.
 */
inline void memory_copy(unsigned char *source, unsigned char *destination, uint32_t nr_bytes);

#endif