// @desc     Processes
// @author   Davide Della Giustina
// @date     15/03/2020

#include "processes.h"

// Ready queue node
typedef struct __ready_queue_node_t {
    process_t *process;
    struct __ready_queue_node_t *next;
} ready_queue_node_t;

extern page_directory_t *kernel_directory; // From paging.c

process_t *init; // Init process
process_t *current_process; // Currently executing process
ready_queue_node_t *ready_queue; // Ready queue

uint32_t next_available_pid = 1; // Next available PID

/* Initialize the structures needed for managing processes.
 */
void processes_init() {
    asm volatile ("cli"); // Temporarily disable interrupts
    init = (process_t *)kmalloc(sizeof(process_t)); // Initialize init process
    init->pid = next_available_pid++;
    init->esp = 0xbfffffff;
    init->ebp = 0xbfffffff;
    init->eip = 0;
    init->page_directory = clone_page_directory(kernel_directory);
    // Add some mapping for text, data and stack sections
    asm volatile ("sti"); // Re-enable interrupts
}

/* Launch the init process.
 */
void launch_init() {
    ready_queue_node_t *first_node = kmalloc(sizeof(ready_queue_node_t));
    ready_queue = first_node;
}

extern void kprint();

/* Perform a context switch.
 */
void context_switch() {
    // TODO
}

/* Fork POSIX call: create a new process.
 * @return              0 to the child process, child PIC to the parent process.
 */
int fork() {
    // TODO
    return 0;
}

/* Execv POSIX call: replace the current process' code with another one.
 * @param program       Program to execute.
 * @param args          Args to pass to the program.
 */
void execv(char *program, char **args) {
    // TODO
    (void)program; (void)args;
}

/* Retrieve the PID of the current process.
 * @return              PID.
 */
int getpid() {
    return current_process->pid;
}