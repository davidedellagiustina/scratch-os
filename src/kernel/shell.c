// TEMP FILE TO AVOID CIRCULAR REFERENCES

#include "shell.h"

extern void print_ascii_art();

/* Parse basic shell commands.
 * @param cmd           Input command.
 */
void parse_input(char *cmd) {
    if (strncmp(cmd, "echo ", 4) == 0) { // ECHO
        int i;
        for (i = 5; i < strlen(cmd); ++i) {
            char c[2];
            c[0] = cmd[i];
            c[1] = 0;
            kprint(c);
        }
        kprint("\n");
    } else if (strcmp(cmd, "clear") == 0) { // CLEAR
        clear_screen();
        print_ascii_art();
        kprint("\n\n");
    } else if (strcmp(cmd, "who") == 0) { // WHO
        kprint("root\n");
    } else if (strcmp(cmd, "shutdown") == 0) { // SHUTDOWN
        kprint("Shutting down the system...\n");
        outw(0x604, 0x2000); // QEMU specific instuction for shutdown
        outw(0xb004, 0x2000); // QEMU (<2.0), Bochs
        outw(0x4004, 0x3400); // Virtualbox
    } else if (strcmp(cmd, "halt") == 0) { // HALT
        kprint("Halting the CPU...\n");
        asm volatile("hlt");
    } else { // UNRECOGNISED COMMAND
        kprint("Unrecognised command\n");
    }
    kprint("> ");
}