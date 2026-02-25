#include <utils/kpanic.h>
#include <utils/kprintf.h>

void kpanic(const char* text) {
	kprintf("Kernel panic!\nReason: %s\n", text);
	for (;;);
}
