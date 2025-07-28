#include <kernel/panic.h>
#include <kernel/console.h>

void kpanic(const char* text) {
	kprintf("Kernel panic!\nError: %s\n", text);

	for(;;);
}