// @desc     Assertions header
// @author   Davide Della Giustina
// @date     29/02/2020

#ifndef ASSERT_H
#define ASSERT_H

#include <stdint.h>
#include "../cpu/panic.h"

/* Check an assertion, panic if false.
 * @param retval    Value returned from function.
 */
void assert(int retval);

#endif