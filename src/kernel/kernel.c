#include <stdio.h>
#include <stdint.h>
#include <kernel/tty.h>
#include <kernel/terminal.h>

void _cdecl kmain() {
	// Инициализация TTY
	tty_initialize();

	// Передача управления терминалу
	terminal_main();

	for(;;);
}
