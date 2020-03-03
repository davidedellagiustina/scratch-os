// TEMP FILE TO AVOID CIRCULAR REFERENCES

#include "shell.h"

/* Parse basic shell commands.
 * @param cmd           Input command.
 */
void parse_input(char *cmd) {
    if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0) {
        kprint("Halting the CPU...");
        asm volatile("hlt");
    } else if (strcmp(cmd, "kmalloc") == 0) {
        void *space = kmalloc(1000);
        char space_str[16] = "";
        itoa((uint32_t)space, space_str, 16);
        kprint("Page: 0x");
        kprint(space_str);
    } else {
        kprint("Unrecognised command");
    }
    kprint("\n> ");
}