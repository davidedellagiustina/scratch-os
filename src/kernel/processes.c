// @desc     Processes
// @author   Davide Della Giustina
// @date     15/03/2020

#include "processes.h"

extern uint32_t read_eip();
extern void p_init_main();
extern void p_init_end();

// Ready queue node
typedef struct __ready_queue_node_t {
    pcb_t *process;
    struct __ready_queue_node_t *next;
} ready_queue_node_t;

extern page_directory_t *kernel_directory; // From paging.c

pcb_t *init; // Init process
pcb_t *current_process = NULL; // Currently executing process
ready_queue_node_t *ready_queue = NULL; // Ready queue
ready_queue_node_t *endof_ready_queue = NULL;

uint32_t next_available_pid = 1; // Next available PID

/* Initialize the structures needed for managing processes.
 */
void processes_init() {
    asm volatile ("cli"); // Temporarily disable interrupts
    // Initialize init process
    init = (pcb_t *)kmalloc(sizeof(pcb_t));
    init->pid = next_available_pid++;
    init->esp = 0xbfffffff;
    init->ebp = 0xbfffffff;
    init->eip = 0x0;
    init->page_directory = clone_page_directory(kernel_directory);
    // Add some mapping for text, data and stack sections
    uint32_t i = 0x0, j = (uint32_t)p_init_main;
    while (i < 0x8000) { // 32KiB for each process' text and data sections
        uint32_t pti = (uint32_t)i >> 22; // Page table index
        uint32_t pi = ((uint32_t)i >> 12) & 0x3ff; // Page index
        if (!init->page_directory->tables[pti]) create_page_table(init->page_directory, pti, 0, 1); // User-mode page table
        physaddr_t addr = alloc_frame(&(init->page_directory->tables[pti]->pages[pi]), 0, 1);
        // Load init process code in memory, for each page it takes
        if (j < (uint32_t)p_init_end) {
            temp_map(addr);
            int nbytes = (((uint32_t)p_init_end - j > 0x1000)? 0x1000 : (uint32_t)p_init_end - j);
            memcpy((void *)j, (void *)0xc3fff000, nbytes);
            temp_demap();
            j += 0x1000;
        }
        // Increment pointer
        i += 0x1000;
    }
    i = 0xbfff8000;
    while (i < 0xc0000000) { // 32KiB for each process' stack section
        uint32_t pti = (uint32_t)i >> 22; // Page table index
        uint32_t pi = ((uint32_t)i >> 12) & 0x3ff; // Page index
        if (!init->page_directory->tables[pti]) create_page_table(init->page_directory, pti, 0, 1); // User-mode page table
        alloc_frame(&init->page_directory->tables[pti]->pages[pi], 0, 1);
        i += 0x1000; // Increment pointer
    }
    asm volatile ("sti"); // Re-enable interrupts
}

/* Launch the init process.
 */
void launch_init() {
    ready_queue_node_t *first_node = kmalloc(sizeof(ready_queue_node_t));
    first_node->process = init;
    first_node->next = NULL;
    endof_ready_queue = first_node;
    ready_queue = first_node;
}

/* Perform a context switch.
 */
void context_switch() {
    if (ready_queue == NULL) return;
    uint32_t eip, ebp, esp;
    asm volatile("mov %%ebp, %0" : "=r"(ebp));
    asm volatile("mov %%esp, %0" : "=r"(esp));
    eip = read_eip();
    if (eip == 0xdeadc0de) return; // Just switched process
    if (current_process != NULL) {
        current_process->eip = eip;
        current_process->ebp = ebp;
        current_process->esp = esp;
    }
    current_process = ready_queue->process;
    // Rotate ready queue
    endof_ready_queue->next = ready_queue;
    endof_ready_queue = endof_ready_queue->next;
    ready_queue = ready_queue->next;
    endof_ready_queue->next = NULL;
    // Switch to the next task
    asm volatile("cli");
    switch_page_directory(current_process->page_directory);
    asm volatile("mov %0, %%ecx" : : "r"(current_process->eip));
    asm volatile("mov %0, %%ebp" : : "r"(current_process->ebp));
    asm volatile("mov %0, %%esp" : : "r"(current_process->esp));
    asm volatile("mov $0xdeadc0de, %eax");
    asm volatile("sti");
    asm volatile("jmp *%ecx");
}

/* Fork POSIX call: create a new process.
 * @return              0 to the child process, child PID to the parent process.
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