// @desc     Low level basic driver: ports in / out.
// @author   Davide Della Giustina
// @date     07/12/2019

#include "ports.h"

/* Read a byte from a port.
 * @param port      Port to read from.
 * @return          Data read from port.
 */
inline uint8_t port_byte_in(uint16_t port) {
    uint8_t result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

/* Write a byte to a port.
 * @param port      Port to write to.
 * @param data      Data to write.
 */
inline void port_byte_out(uint16_t port, uint8_t data) {
    __asm__ __volatile__("out %%al, %%dx" : : "a" (data), "d" (port));
}

/* Read a word from a port.
 * @param port      Port to read from.
 * @return          Data read from port.
 */
inline uint16_t port_word_in(uint16_t port) {
    uint16_t result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

/* Write a word to a port.
 * @param port      Port to write to.
 * @param data      Data to write.
 */
inline void port_word_out(uint16_t port, uint16_t data) {
    __asm__ __volatile__("out %%ax, %%dx" : : "a" (data), "d" (port));
}