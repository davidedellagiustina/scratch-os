// @desc     Ordered array header
// @author   Davide Della Giustina
// @date     29/02/2020

#ifndef ORDERED_ARRAY_H
#define ORDERED_ARRAY_H

#include <stdint.h>
#include "../libc/assert.h"

typedef void* type_t; // Generic type

// A less-than predicate should return nonzero if the first argument is less than the second, else it should return zero.
typedef int8_t (*lessthan_predicate_t)(type_t, type_t);

typedef struct {
    type_t *array;
    uint32_t size;
    uint32_t max_size;
    lessthan_predicate_t less_than;
} ordered_array_t;

/* A standard less-than predicate.
 * @param a         First element to compare.
 * @param b         Second element to compare.
 * @return          Zero if a >= b, nonzero otherwise.
 */
int8_t standard_lessthan_predicate(type_t a, type_t b);

/* Create a new ordered array and place it in memory using kmalloc().
 * @param max_size  Maximum size of the array.
 * @param less_than Less-than predicate to be used.
 * @return          Ordered array.
 */
ordered_array_t create_ordered_array(uint32_t max_size, lessthan_predicate_t less_than);

/* Create a new ordered array and place it in memory at the given (virtual) address.
 * @param addr      Address.
 * @param max_size  Maximum size of the array.
 * @param less_than Less-than predicate to be used.
 */
ordered_array_t place_ordered_array(void *addr, uint32_t max_size, lessthan_predicate_t less_than);

/* Destroy an ordered array and delete it from memory.
 * @param array     Ordered array to be destroyed.
 */
void destroy_ordered_array(ordered_array_t *array);

/* Insert an element in an ordered array.
 * @param array     Ordered array.
 * @param item      New item to insert.
 */
void insert_ordered_array(ordered_array_t *array, type_t item);

/* Get the element at a certain position in an ordered array.
 * @param array     Ordered array.
 * @param i         Index to look up.
 * @return          Element at position #i.
 */
type_t get_ordered_array(ordered_array_t *array, uint32_t i);

/* Remove an element from an ordered array.
 * @param array     Ordered array.
 * @param i         Index of the element to be remove.
 */
void remove_ordered_array(ordered_array_t *array, uint32_t i);

#endif