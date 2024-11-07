#include <kernel/terminal.h>
#include <kernel/tty.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <utils.h>

bool help() {
	printf("HELP:\n\nhelp\ninfo\necho\nclear\n");
	return true;
}

bool info() {
	printf("INFO:\n\nSystem: Minio\nVersion: 0.1\nAuthor: Truzme_\n");
	return true;
}

bool echo(char* command) {
	if (strcmp(string_split(command, ' ', 1), command) == 0) {
		printf("Incorrect command format!\n");
		printf("Example: echo <text>\n");
		return false;
	}

	for (int i = 5; i < strlen(command); i++) {
		putchar(command[i]);
	}

	putchar('\n');
	putchar('\r');

	return true;
}

bool clear() {
	tty_clear();
	return true;
}

void terminal_main() {
	char* command;
	char far* arg_1;

	printf("Welcome to Minio!\n\n");

	while (true) {
		printf("> ");

		command = tty_gets();

		arg_1 = (char far*) string_split(command, ' ', 0);

		if (strcmp(arg_1, "help") == 0) {
			help();
		} else if (strcmp(arg_1, "info") == 0) {
			info();
		} else if (strcmp(arg_1, "echo") == 0) {
			echo(command);
		} else if (strcmp(arg_1, "clear") == 0) {
			clear();
		} else {
			printf("Unknown command\n");
		}
	}
}
