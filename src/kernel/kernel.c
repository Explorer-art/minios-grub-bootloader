#include <stdio.h>
#include <stdint.h>
#include <kernel/tty.h>

void _cdecl kmain() {
	// Инициализация терминала
	terminal_initialize();

	// Передача управления терминалу
	terminal_loop();
}
