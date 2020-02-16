// @desc     String-related functions
// @author   Davide Della Giustina
// @date     07/01/2020

#include "string.h"

/* Convert an integer value to an ASCII string.
 * @param n             Integer value.
 * @param str           String to put the value to.
 * @param base          Base the integer value should be interpreted in (max 16).
 * @return              Pointer to string (#str).
 */
char *itoa(int n, char *str, int base) {
    if (base < 2 || base > 16) {
        str[0] = '\0';
        return str;
    }
    char alph[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' }; // Alphabet
    int i, sign;
    if (base == 10 && (sign = n) < 0) n = -n; // Signed mode is useful just when working in base 10
    unsigned int m = (unsigned int)n; // Unsigned copy of n, to make calculations
    i = 0;
    do {
        str[i++] = alph[m % base];
    } while ((m /= base) > 0);
    if (base == 10 && sign < 0) str[i++] = '-'; // Signed mode is useful just when working in base 10
    str[i] = '\0';
    str_reverse(str);
    return str;
}

/* Convert an ASCII string to an integer value.
 * @param str           String.
 * @return              Integer value.
 */
int atoi(const char *str) {
    int r = 0, i;
    for (i = 0; str[i] != '\0'; ++i)
        r = r * 10 + str[i] - '0';
    return r;
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
    for (i = 0; str1[i] == str2[i]; ++i)
        if (str1[i] == '\0') return 0;
    return (str1[i] - str2[i]);
}

/* Compare two strings until a certain character (position #n).
 * @param str1          First string.
 * @param str2          Second string.
 * @param n             Position of the last character to compare.
 * @return              Negative => str1< str2, zero => str1 = str2, positive => str1 > str2.
 */
int strncmp(const char *str1, const char *str2, int n) {
    int i;
    for (i = 0; n && str1[i] == str2[i]; ++i, --n)
        if (str1[i] == '\0') return 0;
    return (str1[i] - str2[i]);
}

/* Copy string #src to #dst.
 * @param src           Source string.
 * @param dst           Destination string.
 * @return              Pointer to #dst.
 */
char *strcpy(const char *src, char * dst) {
    char *ptr = dst;
    while (*src != '\0')
        *ptr++ = *src++;
    *ptr = '\0';
    return dst;
}

/* Copy string #src to #dst, but just the first #n characters.
 * @param src           Source string.
 * @param dst           Destination string.
 * @param n             Number of characters to copy.
 * @return              Pointer to @dst.
 */
char *strncpy(const char *src, char *dst, int n) {
    char *ptr = dst;
    while (*src != '\0' && n--)
        *ptr++ = *src++;
    *ptr = '\0';
    return dst;
}

/* Concat #src at the end of #dst.
 * @param src           String to be appended.
 * @param dst           String to append to.
 * @return              Pointer to @dst.
 */
char *strcat(char *dst, const char *src) {
    char *ptr = dst + strlen(dst);
    while (*src != '\0')
        *ptr++ = *src++;
    *ptr = '\0';
    return dst;
}

/* Concat first #n characters of #src at the end of #dst.
 * @param src           String to be appended.
 * @param dst           String to append to.
 * @param n             Number of characters to append.
 * @return              Pointer to #dst.
 */
char *strncat(char *dst, const char *src, int n) {
    char *ptr = dst + strlen(dst);
    while (*src != '\0' && n--)
        *ptr++ = *src++;
    *ptr = '\0';
    return dst;
}

/* Reverse a string.
 * @param str           String to reverse.
 * @return              Pointer to #str.
 */
char *str_reverse(char *str) {
    int c, i, j;
    for (i = 0, j = strlen(str)-1; i < j; ++i, --j) {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
    return str;
}

/* Delete the last #n characters of a string.
 * @param str           String.
 * @param n             Number of characters to truncate.
 * @return              Pointer to #str.
 */
char *str_truncate(char *str, int n) {
    if (n <= 0) return str;
    int l = n;
    int len = strlen(str);
    if (n > len) l = len;
    str[len-l] = '\0';
    return str;
}