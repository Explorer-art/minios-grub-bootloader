#include <scheduler.h>
#include <x86.h>
#include <utils/kprintf.h>
#include <stddef.h>

process_context_t sheduler_context;
process_context_t* sheduler_context_ptr = &sheduler_context;

process_t* current_process = NULL;

void schedule(void) {    
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (ptable.process[i].state == PROCESS_STATE_RUNNABLE) {
            process_t* prev = NULL;

            prev = current_process;
            current_process = &ptable.process[i];
            current_process->state = PROCESS_STATE_RUNNING;

            vmm_switch_user_page_directory(current_process->page_directory);

            if (!prev) {
                switch_context(&sheduler_context_ptr, current_process->stack_pointer);
            }
        }
    }
}

void yield(void) {
    // vmm_switch_kernel_page_directory();
    schedule();
}