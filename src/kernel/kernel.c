#include <kernel/tty.h>

void kmain() {
	terminal_initialize();

	terminal_writestring("Hello World!");

	for(;;);
}