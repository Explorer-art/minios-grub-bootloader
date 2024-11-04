#pragma once

#define BUFFER_SIZE 1024
#define ENTER 0x0D
#define SPACE 0x20
#define BACKSPACE 0x08

void tty_initialize();
void tty_putchar(char c);
void tty_buffer_clear();
char tty_getchar();
char* tty_gets();
