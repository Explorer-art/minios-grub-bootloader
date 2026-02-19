#include <process.h>
#include <mm/vmm.h>
#include <mm/pmm.h>
#include <utils/kprintf.h>
#include <memory.h>

static process_table_t ptable;
static uint32_t next_pid = 1;

process_t* process_create() {
    process_t* p;
    char* sp;

    for (p = ptable; p < &ptable[MAX_PROCESSES]; p++) {
        if (p->state == PROCESS_STATE_UNUSED) {
            goto found;
        }
    }

    kprint("warning: Too many processes\n");
    return NULL;

found:
    memset(p, 0, sizeof(process_t));
    p->state = PROCESS_STATE_EMBRYO;
    p->pid = next_pid++;

    if ((p->kernel_stack = pmm_alloc_page()) == NULL) {
        p->state = PROCESS_STATE_UNUSED;
        return NULL;
    }

    sp = p->kernel_stack + KERNEL_STACK_SIZE;

    sp -= sizeof(trapframe_t);
    p->tf = (trapframe_t*)sp;

    sp -= sizeof(uint32_t);
    *(uint32_t*)sp = (uint32_t)trapret;

    sp -= sizeof(process_context_t);
    p->context = (process_context_t*)sp;
    memset(p->context, 0, sizeof(process_context_t));
    p->context->eip = (uint32_t)forkret;

    return p;
}

void process_terminate(uint8_t pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (ptable[i].pid == pid) {
            ptable[i].state = PROCESS_STATE_UNUSED;
            break;
        }
    }
}
