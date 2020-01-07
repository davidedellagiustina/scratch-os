// @desc     Low level basic driver: ports in / out
// @author   Davide Della Giustina
// @date     07/12/2019

#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>

/* Read a byte from a port.
 * @param port      Port to read from.
 * @return          Data read from port.
 */
inline uint8_t inb(uint16_t port);

/* Write a byte to a port.
 * @param port      Port to write to.
 * @param data      Data to write.
 */
inline void outb(uint16_t port, uint8_t data);

/* Read a word from a port.
 * @param port      Port to read from.
 * @return          Data read from port.
 */
inline uint16_t inw(uint16_t port);

/* Write a word to a port.
 * @param port      Port to write to.
 * @param data      Data to write.
 */
inline void outw(uint16_t port, uint16_t data);

#endif