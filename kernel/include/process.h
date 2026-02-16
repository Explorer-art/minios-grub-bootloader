#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <mm/vmm.h>

#define MAX_PROCESSES 64
#define KERNEL_STACK_SIZE 8192

typedef enum {
    PROCESS_STATE_READY,
    PROCESS_STATE_RUNNING,
    PROCESS_STATE_BLOCKED,
    PROCESS_STATE_WAITING,
    PROCESS_STATE_TERMINATED
} process_state_t;

typedef struct {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;
    uint32_t esp; // Stack pointer
    uint32_t eip; // Instruction pointer
    uint32_t eflags; // EFLAGS register
} process_context_t;

typedef struct {
    uint32_t pid;
    char name[32];

    process_context_t context;
    process_state_t state;

    page_directory_t* page_directory;
    uint32_t stack;
    uint32_t stack_size;
    uint32_t kernel_stack;
    uint32_t kernel_stack_size;
} __attribute__((packed)) process_t;

process_t* process_create(char* name);
void process_terminate(uint8_t pid);

#endif
