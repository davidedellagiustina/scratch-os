// TEMP FILE JUST TO AVOID CIRCULAR REFERENCES

#ifndef SHELL_H
#define SHELL_H

#include "../drivers/vga.h"
#include "../libc/mem.h"
#include "../libc/string.h"

#include "heap.h"

/* Parse basic shell commands.
 * @param cmd           Input command.
 */
void parse_input(char *cmd);

#endif