// @desc     Low level basic driver: ports in / out.
// @author   Davide Della Giustina
// @date     07/12/2019

#ifndef PORTS_H
#define PORTS_H

/* Read a byte from a port.
 * @param port      Port to read from.
 * @return          Data read from port.
 */
unsigned char port_byte_in(unsigned short port);

/* Write a byte to a port.
 * @param port      Port to write to.
 * @param data      Data to write.
 */
void port_byte_out(unsigned short port, unsigned char data);

/* Read a word from a port.
 * @param port      Port to read from.
 * @return          Data read from port.
 */
unsigned short port_word_in(unsigned short port);

/* Write a word to a port.
 * @param port      Port to write to.
 * @param data      Data to write.
 */
void port_word_out(unsigned short port, unsigned short data);

#endif