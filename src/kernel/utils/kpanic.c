#include <kernel/utils/kpanic.h>
#include <kernel/utils/kprintf.h>

void kpanic(const char* text) {
	kprintf("Kernel panic!\nReason: %s\n", text);

	for(;;);
}
