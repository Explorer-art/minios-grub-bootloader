#include <kernel/tty.h>
#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <memdefs.h>
#include <string.h>
#include <utils.h>
#include <kernel/io.h>

#define BUFFER_SIZE 1024
#define ENTER 0x0D
#define SPACE 0x20
#define BACKSPACE 0x08

static bool gets_enable = false;
static char buffer[BUFFER_SIZE];

void terminal_buffer_clear() {
	memset(buffer, '\0', BUFFER_SIZE);
}

char terminal_getchar() {
	return read_char();
}

char* terminal_gets() {
	int i = 0;
	char c;

	terminal_buffer_clear();

	while (true) {
		c = terminal_getchar();

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

void terminal_loop() {
	char* command;
	char far* arg_1;

	while (true) {
		printf("> ");

		command = terminal_gets();

		arg_1 = (char far*) string_split(command, ' ', 0);

		if (strcmp(arg_1, "help") == 0) {
			printf("HELP:\n\nhelp\ninfo\necho\n");
		} else {
			printf("Unknown command\n");
		}
	}
}

void terminal_initialize() {
	terminal_buffer_clear();
}
