// @desc     String-related functions header
// @author   Davide Della Giustina
// @date     07/01/2020

#ifndef STRINGS_H
#define STRINGS_H

#include <stdint.h>

/* Convert an integer value to an ASCII string.
 * @param n             Integer value.
 * @param str           String to put the value to.
 * @param base          Base the integer value should be interpreted in.
 */
void itoa(int n, char str[], int base);

/* Reverse a string.
 * @param str           String to reverse.
 */
void reverse(char str[]);

/* Compute the length of a string.
 * @param str           String.
 * @return              Length of the string.
 */
int strlen(char str[]);

/* Delete the last character of a string.
 * @param str           String.
 */
void backspace(char str[]);

/* Append a character at the end of a string.
 * @param str           String.
 * @param c             Character.
 */
void append(char str[], char c);

/* Compare two strings.
 * @param str1          First string.
 * @param str2          Second string.
 * @return              Negative => str1 < str2, zero => str1 = str2, positive => str1 > str2.
 */
int strcmp(char str1[], char str2[]);

#endif