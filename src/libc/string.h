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
 * @return              Pointer to string (#str).
 */
char *itoa(int n, char *str, int base);

/* Convert an ASCII string to an integer value.
 * @param str           String.
 * @return              Integer value.
 */
int atoi(const char *str); // TODO

/* Compute the length of a string.
 * @param str           String.
 * @return              Length of the string.
 */
int strlen(const char *str);

/* Compare two strings.
 * @param str1          First string.
 * @param str2          Second string.
 * @return              Negative => str1 < str2, zero => str1 = str2, positive => str1 > str2.
 */
int strcmp(const char *str1, const char *str2);

/* Compare two strings until a certain character (position #n).
 * @param str1          First string.
 * @param str2          Second string.
 * @param n             Position of the last character to compare.
 * @return              Negative => str1< str2, zero => str1 = str2, positive => str1 > str2.
 */
int strncmp(const char *str1, const char *str2, int n); // TODO

/* Copy string #src to #dst.
 * @param src           Source string.
 * @param dst           Destination string.
 * @return              Pointer to #dst.
 */
char *strcpy(const char *src, char * dst); // TODO

/* Copy string #src to #dst, but just the first #n characters.
 * @param src           Source string.
 * @param dst           Destination string.
 * @param n             Number of characters to copy.
 * @return              Pointer to @dst.
 */
char *strncpy(const char *src, char *dst, int n); // TODO

/* Reverse a string.
 * @param str           String to reverse.
 */
void reverse(char *str);

/* Delete the last character of a string.
 * @param str           String.
 */
void backspace(char *str);

/* Append a character at the end of a string.
 * @param str           String.
 * @param c             Character.
 */
void append(char *str, char c);

#endif