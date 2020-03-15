// @desc     Processes header
// @author   Davide Della Giustina
// @date     15/03/2020

#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdint.h>
#include "../cpu/paging.h"
#include "heap.h"

// Represent a process control block
typedef struct {
    int pid; // Process ID
    uint32_t esp, ebp; // Stack pointers
    uint32_t eip; // Instruction pointer
    page_directory_t *page_directory; // Address space (page directory)
} process_t;

/* Initialize the structures needed for managing processes.
 */
void processes_init();

/* Launch the init process.
 */
void launch_init();

/* Perform a context switch.
 */
void context_switch();

/* Fork POSIX call: create a new process.
 * @return              0 to the child process, child PIC to the parent process.
 */
int fork();

/* Execv POSIX call: replace the current process' code with another one.
 * @param program       Program to execute.
 * @param args          Args to pass to the program.
 */
void execv(char *program, char **args);

/* Retrieve the PID of the current process.
 * @return              PID.
 */
int getpid();

#endif