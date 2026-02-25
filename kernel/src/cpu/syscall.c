#include <cpu/syscall.h>
#include <drivers/tty.h>
#include <drivers/keyboard.h>
#include <mm/vmm.h>
#include <process.h>
#include <scheduler.h>
#include <utils/kprintf.h>
#include <stddef.h>

extern process_t* current_process;
extern page_directory_t* kpage_directory;

/*
Read syscall
Parameters:
- ebx: source
- ecx: destination
*/

uint8_t syscall_read(registers_t* regs) {
	uint8_t c;

	if (regs->ebx == 0) {
		c = keyboard_getchar();
	}

	regs->ecx = (uint32_t)c;

	return 0;
}

/*
Write syscall
Parameters:
- ebx: source
- ecx: destination
*/

uint8_t syscall_write(registers_t* regs) {
	if (regs->ecx == 1)
		tty_puts((const char*)regs->ebx);
	else
		return 1;

	return 0;
}

/*
Exit syscall
Parameters: none
*/

uint8_t syscall_exit(registers_t* regs) {
	vmm_switch_kernel_page_directory(current_process->kernel_stack);
	process_terminate(current_process);
	current_process = NULL;
	yield();
	return 0;
}

static syscall_t syscalls[SYSCALL_COUNT] = {
	syscall_read,
	syscall_write,
	syscall_exit
};

void syscall_handler(registers_t* regs) {
	if (regs->eax >= SYSCALL_COUNT)
		return;

	syscall_t syscall = syscalls[regs->eax];

	syscall(regs);
}
