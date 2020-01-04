// @desc     Kernel useful functions
// @author   Davide Della Giustina
// @date     08/12/2019

#include "utils.h"

/* Copy a portion of memory from a source to a destination.
 * @param source        Source address.
 * @param destination   Destination address.
 * @param nr_bytes      Number of bytes to copy.
 */
void memory_copy(unsigned char *source, unsigned char *destination, int32_t nr_bytes) {
    int32_t i;
    for (i = 0; i < nr_bytes; ++i) {
        *(destination + i) = *(source + i);
    }
}

/* Set a certain portion of memory to a value.
 * @param dest          Base memory address.
 * @para, val           Value to set.
 * @param len           Length of memory portion.
 */
void memory_set(uint8_t *dest, uint8_t val, int32_t len) {
    uint8_t *temp = dest;
    for (; len != 0; --len) *temp++ = val;
}

/* Swap two 8 bit integers in memory.
 * @param a             First parameter.
 * @param b             Second parameter.
 */
void swap(uint8_t *a, uint8_t *b) {
    uint8_t tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Compute the length of a string.
 * @param str           String.
 */
int32_t strlen (char str[]) {
    uint32_t i = 0;
    while (str[i] != '\0') ++i;
    return i;
}

/* Reverse a string.
 * @param str           String to reverse.
 */
void reverse(char str[]) {
    int32_t start = 0, end = strlen(str) - 1;
    while (start < end) {
        swap(str+start, str+end);
        ++start; --end;
    }
}

/* Convert an integer value to an ASCII string.
 * @param n             Integer value.
 * @param str           String to put the value to.
 */
void int_to_ascii(int32_t n, char str[]) {
    int32_t i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    if (sign < 0) str[i++] = '-';
    str[i] = '\0';
    reverse(str);
}