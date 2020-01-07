// @desc     Low level basic driver: ports in / out
// @author   Davide Della Giustina
// @date     07/12/2019

#include "ports.h"

/* Read a byte from a port.
 * @param port      Port to read from.
 * @return          Data read from port.
 */
inline uint8_t inb(uint16_t port) {
    uint8_t result;
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

/* Write a byte to a port.
 * @param port      Port to write to.
 * @param data      Data to write.
 */
inline void outb(uint16_t port, uint8_t data) {
    asm volatile("out %%al, %%dx" : : "a" (data), "d" (port));
}

/* Read a word from a port.
 * @param port      Port to read from.
 * @return          Data read from port.
 */
inline uint16_t inw(uint16_t port) {
    uint16_t result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

/* Write a word to a port.
 * @param port      Port to write to.
 * @param data      Data to write.
 */
inline void outw(uint16_t port, uint16_t data) {
    asm volatile("out %%ax, %%dx" : : "a" (data), "d" (port));
}