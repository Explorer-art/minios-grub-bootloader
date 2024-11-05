#include <kernel/tty.h>
#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <kernel/io.h>

static bool gets_enable = false;
static char buffer[BUFFER_SIZE];

void tty_clear() {
	clear_screen();
}

void tty_putchar(char c) {
	write_char(c);
}

char tty_getchar() {
	return read_char();
}

void tty_buffer_clear() {
	memset(buffer, '\0', BUFFER_SIZE);
}

char* tty_gets() {
	int i = 0;
	char c;

	tty_buffer_clear();

	while (true) {
		c = tty_getchar();

		if (c == ENTER || i >= BUFFER_SIZE) {
			tty_putchar('\n');
			tty_putchar('\r');
			break;
		} else if (c == BACKSPACE) {
			if (i > 0) {
				tty_putchar(BACKSPACE);
				tty_putchar(SPACE);
				tty_putchar(BACKSPACE);
				i--;
				buffer[i] = '\0';
			}
		} else {
			write_char(c);
			buffer[i] = c;
			i++;
		}
	}

	return buffer;
}

void tty_initialize() {
	tty_clear();
	tty_buffer_clear();
}
