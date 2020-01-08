// @desc     String-related functions
// @author   Davide Della Giustina
// @date     07/01/2020

#include "string.h"

/* Convert an integer value to an ASCII string.
 * @param n             Integer value.
 * @param str           String to put the value to.
 * @param base          Base the integer value should be interpreted in.
 * @return              Pointer to string (#str).
 */
char *itoa(int n, char *str, int base) {
    if (base < 2 || base > 16) {
        str[0] = '\0';
        return str;
    }
    char alph[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' }; // Alphabet
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = alph[n % base];
    } while ((n /= base) > 0);
    if (sign < 0) str[i++] = '-';
    str[i] = '\0';
    reverse(str);
    return str;
}

/* Compute the length of a string.
 * @param str           String.
 * @return              Length of the string.
 */
int strlen(const char *str) {
    int i = 0;
    while (str[i] != '\0') ++i;
    return i;
}

/* Compare two strings.
 * @param str1          First string.
 * @param str2          Second string.
 * @return              Negative => str1 < str2, zero => str1 = str2, positive => str1 > str2.
 */
int strcmp(const char *str1, const char *str2) {
    int i;
    for (i = 0; str1[i] == str2[i]; ++i) {
        if (str1[i] == '\0') return 0;
    }
    return (str1[i] - str2[i]);
}

/* Reverse a string.
 * @param str           String to reverse.
 */
void reverse(char *str) {
    int c, i, j;
    for (i = 0, j = strlen(str)-1; i < j; ++i, --j) {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
}

/* Delete the last character of a string.
 * @param str           String.
 */
void backspace(char *str) {
    int len = strlen(str);
    str[len-1] = '\0';
}

/* Append a character at the end of a string.
 * @param str           String.
 * @param c             Character.
 */
void append(char *str, char c) {
    int len = strlen(str);
    str[len] = c;
    str[len+1] = '\0';
}