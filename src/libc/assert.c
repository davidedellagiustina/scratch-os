// @desc     Assertions
// @author   Davide Della Giustina
// @date     29/02/2020

#include "assert.h"

/* Check an assertion, panic if false.
 * @param retval    Value returned from function.
 */
void assert(int retval) {
    if (!retval) panic("assertion failed");
}