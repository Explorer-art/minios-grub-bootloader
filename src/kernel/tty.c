#include <kernel/tty.h>
#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <kernel/io.h>

static bool gets_enable = false;
static char buffer[BUFFER_SIZE];

void tty_putchar(char c) {
	write_char(c, 0);
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
			write_char('\n', 0);
			write_char('\r', 0);
			break;
		} else if (c == BACKSPACE) {
			if (i > 0) {
				write_char(BACKSPACE, 0);
				write_char(SPACE, 0);
				write_char(BACKSPACE, 0);
				i--;
				buffer[i] = '\0';
			}
		} else {
			write_char(c, 0);
			buffer[i] = c;
			i++;
		}
	}

	return buffer;
}

void tty_initialize() {
	tty_buffer_clear();
}
