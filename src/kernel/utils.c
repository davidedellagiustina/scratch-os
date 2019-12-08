// @desc     Kernel useful functions.
// @author   Davide Della Giustina
// @date     08/12/2019

/* Copy a portion of memory from a source to a destination.
 * @param source        Source address.
 * @param destination   Destination address.
 * @param nr_bytes      Number of bytes to copy.
 */
void memory_copy(char *source, char *destination, int nr_bytes) {
    int i;
    for (i = 0; i < nr_bytes; ++i) {
        *(destination + i) = *(source + i);
    }
}