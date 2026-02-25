#include <scheduler.h>
#include <x86.h>
#include <utils/kprintf.h>
#include <stddef.h>
#include <stdbool.h>

bool scheduler_running = false;
process_t* current_process = NULL;

void scheduler_init(void) {
	scheduler_running = !scheduler_running;
	schedule();
}

void schedule(void) {
	if (!scheduler_running) return false;
	   
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (ptable.process[i].state == PROCESS_STATE_RUNNABLE) {
            process_t* prev = NULL;

            prev = current_process;
            current_process = &ptable.process[i];
            current_process->state = PROCESS_STATE_RUNNING;

            vmm_switch_user_page_directory(current_process->page_directory);

            if (!prev) {
                switch_context(NULL, current_process->stack_pointer);
            }
        }
    }

    scheduler_running = false;
    kprintf("System halted!\n");
    kprintf("Reason: There is not a single running process\n");
    for (;;);
}

void yield(void) {
    schedule();
}
