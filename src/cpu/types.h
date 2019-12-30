// @desc     Some useful custom types.
// @author   Davide Della Giustina
// @date     30/12/2019

#ifndef TYPES_H
#define TYPES_H

#define low_16(address)         (uint16_t)(address & 0xffff)
#define high_16(address)        (uint16_t)((address >> 16) & 0xffff)

typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned char uint8_t;
typedef char int8_t;

#endif