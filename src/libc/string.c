// @desc     String-related functions
// @author   Davide Della Giustina
// @date     07/01/2020

#include "string.h"

/* Convert an integer value to an ASCII string.
 * @param n             Integer value.
 * @param str           String to put the value to.
 */
void itoa(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    if (sign < 0) str[i++] = '-';
    str[i] = '\0';
    reverse(str);
}

/* Reverse a string.
 * @param str           String to reverse.
 */
void reverse(char str[]) {
    int c, i, j;
    for (i = 0, j = strlen(str)-1; i < j; ++i, --j) {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
}

/* Compute the length of a string.
 * @param str           String.
 * @return              Length of the string.
 */
int strlen(char str[]) {
    int i = 0;
    while (str[i] != '\0') ++i;
    return i;
}

/* Delete the last character of a string.
 * @param str           String.
 */
void backspace(char str[]) {
    int len = strlen(str);
    str[len-1] = '\0';
}

/* Append a character at the end of a string.
 * @param str           String.
 * @param c             Character.
 */
void append(char str[], char c) {
    int len = strlen(str);
    str[len] = c;
    str[len+1] = '\0';
}

/* Compare two strings.
 * @param str1          First string.
 * @param str2          Second string.
 * @return              Negative => str1 < str2, zero => str1 = str2, positive => str1 > str2.
 */
int strcmp(char str1[], char str2[]) {
    int i;
    for (i = 0; str1[i] == str2[i]; ++i) {
        if (str1[i] == '\0') return 0;
    }
    return (str1[i] - str2[i]);
}