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
void memory_copy(unsigned char *source, unsigned char *destination, int32_t nr_bytes);

/* Set a certain portion of memory to a value.
 * @param dest          Base memory address.
 * @para, val           Value to set.
 * @param len           Length of memory portion.
 */
void memory_set(uint8_t *dest, uint8_t val, int32_t len);

/* Swap two 8 bit integers in memory.
 * @param a             First parameter.
 * @param b             Second parameter.
 */
void swap(uint8_t *a, uint8_t *b);

/* Reverse a string.
 * @param str           String to reverse.
 * @param length        Length of characters to reverse.
 */
void str_reverse(char str[], int32_t length);

/* Convert an integer value to an ASCII string.
 * @param n             Integer value.
 * @param str           String to put the value to.
 */
void int_to_ascii(int32_t n, char str[]);

#endif