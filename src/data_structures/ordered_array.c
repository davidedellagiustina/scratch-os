// @desc     Ordered array
// @author   Davide Della Giustina
// @date     29/02/2020

#include "ordered_array.h"

/* A standard less-than predicate.
 * @param a         First element to compare.
 * @param b         Second element to compare.
 * @return          Zero if a >= b, nonzero otherwise.
 */
int8_t standard_lessthan_predicate(void *a, void *b) {
    return ((a < b)? 1 : 0);
}

/* Create a new ordered array and place it in memory using kmalloc().
 * @param max_size  Maximum size of the array.
 * @param less_than Less-than predicate to be used.
 * @return          Ordered array.
 */
ordered_array_t create_ordered_array(uint32_t max_size, lessthan_predicate_t less_than) {
    ordered_array_t a;
    a.array = (void *)kcalloc(max_size * sizeof(void *));
    a.size = 0; // No elements yet
    a.max_size = max_size;
    a.less_than = less_than;
    return a;
}

/* Create a new ordered array and place it in memory at the given (virtual) address.
 * @param addr      Address.
 * @param max_size  Maximum size of the array.
 * @param less_than Less-than predicate to be used.
 */
ordered_array_t place_ordered_array(void *addr, uint32_t max_size, lessthan_predicate_t less_than) {
    ardered_array_t a;
    a.array = (void *)addr;
    memset(a.array, 0, max_size * sizeof(void *));
    a.size = 0;
    a.max_size = max_size;
    a.less_than = less_than;
    return a;
}

/* Destroy an ordered array and delete it from memory.
 * @param array     Ordered array to be destroyed.
 */
void destroy_ordered_array(ordered_array_t *array) {
    kfree(array->array);
}

/* Insert an element in an ordered array.
 * @param array     Ordered array.
 * @param item      New item to insert.
 */
void insert_ordered_array(ordered_array_t *array, void *item) {
    ASSERT(array->less_than); // There must be a predicate
    ASSERT(array->size < array->max_size); // There must be space for inserting a new element
    uint32_t iterator = 0;
    while (iterator < array->size && array->less_than(array->array[iterator], item)) ++iterator; // Reach inserting position
    if (iterator == array->size) { // If end of array reached
        array->array[array->size++] = item;
    } else { // Otherwise
        void *tmp = array->array[iterator];
        array->array[iterator] = item;
        while (iterator < array->size) {
            ++iterator;
            void *tmp2 = array->array[iterator];
            array->array[iterator] = tmp;
            tmp = tmp2;
        }
        ++array->size;
    }
}

/* Get the element at a certain position in an ordered array.
 * @param array     Ordered array.
 * @param i         Index to look up.
 * @return          Element at position #i.
 */
void *get_ordered_array(ordered_array_t *array, uint32_t i) {
    ASSERT(i < array->size);
    return array->array[i];
}

/* Remove an element from an ordered array.
 * @param array     Ordered array.
 * @param i         Index of the element to be remove.
 */
void remove_ordered_array(ordered_array_t *array, uint32_t i) {
    ASSERT(i < array->size);
    while (i < array->size) {
        array->array[i] = array->array[i+1];
        ++i;
    }
    --array->size;
}