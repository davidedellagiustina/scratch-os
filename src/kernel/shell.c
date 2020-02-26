// TEMP FILE TO AVOID CIRCULAR REFERENCES

#include "shell.h"

/* Parse basic shell commands.
 * @param cmd           Input command.
 */
void parse_input(char *cmd) {
    if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0) {
        kprint("Halting the CPU...");
        asm volatile("hlt");
    } else if (strcmp(cmd, "mkpage") == 0) {
        void *page;
        physaddr_t physical;
        page = kmalloc(1000, 1, &physical);
        char page_str[16] = "", physical_str[16] = "";
        itoa((physaddr_t)page, page_str, 16);
        itoa(physical, physical_str, 16);
        kprint("Page: 0x");
        kprint(page_str);
        kprint(", physical address: 0x");
        kprint(physical_str);
    } else {
        kprint("Unrecognised command");
    }
    kprint("\n> ");
}