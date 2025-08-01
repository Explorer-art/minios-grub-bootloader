#include <kernel/syscalls.h>
#include <kernel/drivers/tty.h>
#include <kernel/drivers/keyboard.h>
#include <kernel/console.h>

/*
Read syscall
Parameters:
- ebx: source
- ecx: mode

Output:
- ax: unsigned char

Modes:
Sigle mode (ECX = 0). Reads one byte per system call.
Block mode (ECX = 1). It can read several bytes at once.
*/

uint8_t read_syscall(registers_t* regs) {
	uint8_t c;

	if (regs->ebx == 0) {
		c = keyboard_getchar();
	}

	return c;
}

/*
Write syscall
Parameters:
- ebx: destination
- ecx: source (byte or pointer)
- edx: mode

Output:
- ax: status
*/

uint8_t write_syscall(registers_t* regs) {
	uint8_t mode;

	if (regs->edx == 0)
		mode = 0;
	else if (regs->edx == 1)
		mode = 1;
	else
		return 1;

	if (regs->ebx == 1 && mode == 0)
		tty_putchar(regs->ecx);
	else if (regs->edx == 1 && mode == 1)
		tty_puts((const char*)regs->ecx);
	else
		return 1;

	return 0;
}

static syscall_t syscalls[SYSCALLS_SIZE] = {
	read_syscall,
	write_syscall
};

void syscalls_handler(registers_t* regs) {
	if (regs->eax >= SYSCALLS_SIZE)
		return;

	syscall_t syscall = syscalls[regs->eax];

	syscall(regs);
}