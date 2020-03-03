// TEMP FILE TO AVOID CIRCULAR REFERENCES

#include "shell.h"

/* Parse basic shell commands.
 * @param cmd           Input command.
 */
void parse_input(char *cmd) {
    if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0) {
        kprint("Halting the CPU...");
        asm volatile("hlt");
    } else {
        kprint("Unrecognised command");
    }
    kprint("\n> ");
}