#include <kernel/gdt.h>
#include <kernel/tty.h>

void kmain() {
	gdt_init();
	terminal_init();

	terminal_writestring("Hello World!");

	for(;;);
}