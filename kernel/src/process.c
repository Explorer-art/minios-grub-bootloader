#include <process.h>
#include <mm/vmm.h>
#include <mm/pmm.h>
#include <x86.h>
#include <utils/kprintf.h>
#include <memory.h>

process_table_t ptable;
uint32_t next_pid = 1;

process_t* process_create(void* program, uint32_t size) {
    process_t* p;
    char *sp_va, *sp_pa;

    for (p = ptable.process; p < &ptable.process[MAX_PROCESSES]; p++) {
        if (p->state == PROCESS_STATE_UNUSED) {
            goto found;
        }
    }

    kprintf("error: Too many processes\n");
    return NULL;

found:
    memset(p, 0, sizeof(process_t));
    p->state = PROCESS_STATE_EMBRYO;
    p->pid = next_pid++;

    if ((p->page_directory = vmm_create_user_page_directory()) == NULL) {
        p->state = PROCESS_STATE_UNUSED;
        return NULL;
    }

    if ((p->kernel_stack = pmm_alloc_page()) == NULL) {
        vmm_destroy_user_page_directory(p->page_directory);
        p->state = PROCESS_STATE_UNUSED;
        return NULL;
    }

    sp_va = KERNEL_STACK_BASE + KERNEL_STACK_SIZE;
    sp_pa = p->kernel_stack + KERNEL_STACK_SIZE;

    if (vmm_mappage(p->page_directory, KERNEL_STACK_BASE, p->kernel_stack, KERNEL_STACK_SIZE, 1, 0, 0)) {
        pmm_free_page(p->kernel_stack);
        vmm_destroy_user_page_directory(p->page_directory);
        p->state = PROCESS_STATE_UNUSED;
        return NULL;
    }

    void* code;

    if ((code = pmm_alloc_page()) == NULL) {
        pmm_free_page(p->kernel_stack);
        vmm_destroy_user_page_directory(p->page_directory);
        // Free stack page table in physical memory
        p->state = PROCESS_STATE_UNUSED;
        return NULL;
    }

    if (vmm_mappage(p->page_directory, USER_PROGRAM_BASE, code, KERNEL_STACK_SIZE, 1, 0, 0)) {
        pmm_free_page(p->kernel_stack);
        vmm_destroy_user_page_directory(p->page_directory);
        // Free stack page table in physical memory
        pmm_free_page(code);
        p->state = PROCESS_STATE_UNUSED;
        return NULL;
    }

    memcpy(code, program, size);

    sp_va -= sizeof(process_context_t);
    sp_pa -= sizeof(process_context_t);

    p->context = (process_context_t*)sp_pa;
    memset(p->context, 0, sizeof(process_context_t));
    p->context->eip = 0x80000000;
    p->stack_pointer = sp_va;

    p->state = PROCESS_STATE_RUNNABLE;

    return p;
}

void process_terminate(process_t* proc) {
    uint32_t pid = proc->pid;
    process_t* p;
    
    for (p = ptable.process; p < &ptable.process[MAX_PROCESSES]; p++) {
        if (ptable.process->pid == pid) {
            cli();
            goto found;
        }
    }

    return;

found:
    p->state = PROCESS_STATE_UNUSED;
    vmm_destroy_user_page_directory(p->page_directory);
    sti();
}
