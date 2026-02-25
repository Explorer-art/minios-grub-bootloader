#include <cpu/irq.h>
#include <x86.h>
#include <utils/kprintf.h>
#include <utils/kpanic.h>

static const char* exception_messages[] = {
	"Divide by zero",
	"Debug",
	"Non-maskable interrupt",
	"Breakpoint",
	"Overflow",
	"Bound range exceeded",
	"Invalid opcode",
	"Device not available",
	"Double fault",
	"Coprocessor segment overrun",
	"Invalid TSS",
	"Segment not present",
	"Stack-segment fault",
	"General protection fault",
	"Page fault",
	"Reserved",
	"x87 floating-point exception",
	"Alignment check",
	"SIMD floating-point exception",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

static irq_handler_t irq_routines[16] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

void isr_handler(registers_t* regs) {
	if (regs->int_no < 32) {
		kpanic(exception_messages[regs->int_no]);
	}
}

void irq_install_handler(uint8_t num, irq_handler_t handler) {
	irq_routines[num] = handler;
}

void irq_uninstall_handler(uint8_t num) {
	irq_routines[num] = 0;
}

void irq_handler(registers_t* regs) {
	irq_handler_t handler = irq_routines[regs->int_no - 32];

	if (handler)
		handler(regs);

	if (regs->int_no >= 40)
		outb(0xA0, 0x20);

	outb(0x20, 0x20);
}
