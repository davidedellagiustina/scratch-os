// @desc     Low level basic driver: ports in / out.
// @author   Davide Della Giustina
// @date     07/12/2019

#ifndef PORTS_C
#define PORTS_C

#include "ports.h"

/* Read a byte from a port.
 * @param port      Port to read from.
 * @return          Data read from port.
 */
unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

/* Write a byte to a port.
 * @param port      Port to write to.
 * @param data      Data to write.
 */
void port_byte_out(unsigned short port, unsigned char data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

/* Read a word from a port.
 * @param port      Port to read from.
 * @return          Data read from port.
 */
unsigned short port_word_in(unsigned short port) {
    unsigned short result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

/* Write a word to a port.
 * @param port      Port to write to.
 * @param data      Data to write.
 */
void port_word_out(unsigned short port, unsigned short data) {
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

#endif