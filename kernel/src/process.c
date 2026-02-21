#include <process.h>
#include <mm/vmm.h>
#include <mm/pmm.h>
#include <utils/kprintf.h>
#include <memory.h>

static process_table_t ptable;
static uint32_t next_pid = 1;

process_t* process_create(void* program, uint32_t size) {
    process_t* p;
    char* sp;

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

    sp = KERNEL_STACK_BASE + KERNEL_STACK_SIZE;

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

    return p;
}

void process_terminate(uint8_t pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (ptable.process[i].pid == pid) {
            ptable.process[i].state = PROCESS_STATE_UNUSED;
            break;
        }
    }
}
