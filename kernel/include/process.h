#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <mm/vmm.h>
#include <spinlock.h>

#define MAX_PROCESSES 10
#define KERNEL_STACK_SIZE 8192

typedef enum {
    PROCESS_STATE_UNUSED,
    PROCESS_STATE_EMBRYO,
    PROCESS_STATE_RUNNABLE,
    PROCESS_STATE_RUNNING
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
    char name[16];
    process_state_t state;
    process_context_t* context;
    page_directory_t* page_directory;
    uint32_t kernel_stack;
    uint32_t kernel_stack_size;
} __attribute__((packed)) process_t;

typedef struct {
    process_t process[MAX_PROCESSES];
    spinlock_t lock;
} __attribute__((packed)) process_table_t;

extern void trapret(void);
extern void forkret(void);

process_t* process_create(char* name);
void process_terminate(uint8_t pid);

#endif
