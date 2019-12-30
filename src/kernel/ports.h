// @desc     Low level basic driver: ports in / out.
// @author   Davide Della Giustina
// @date     07/12/2019

#ifndef PORTS_H
#define PORTS_H

#include "../cpu/types.h"

/* Read a byte from a port.
 * @param port      Port to read from.
 * @return          Data read from port.
 */
inline uint8_t port_byte_in(uint16_t port);

/* Write a byte to a port.
 * @param port      Port to write to.
 * @param data      Data to write.
 */
inline void port_byte_out(uint16_t port, uint8_t data);

/* Read a word from a port.
 * @param port      Port to read from.
 * @return          Data read from port.
 */
inline uint16_t port_word_in(uint16_t port);

/* Write a word to a port.
 * @param port      Port to write to.
 * @param data      Data to write.
 */
inline void port_word_out(uint16_t port, uint16_t data);

#endif