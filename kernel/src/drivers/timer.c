#include <drivers/timer.h>
#include <cpu/irq.h>
#include <cpu/port.h>
#include <utils/kprintf.h>

uint32_t tick = 0;

void timer_handler(registers_t* regs) {
	kprintf("Timer: %d\n", tick);
	tick++;
}

void timer_init(uint32_t frequency) {
	irq_install_handler(0, timer_handler);

	uint32_t divisor = 1193180 / frequency;

	outb(0x43, 0x36);

	uint8_t lower = (uint8_t)(divisor & 0xFF);
	uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

	outb(0x40, lower);
	outb(0x40, high);
}
