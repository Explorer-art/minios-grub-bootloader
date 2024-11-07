#pragma once

#define BUFFER_SIZE 1024
#define ENTER 0x0D
#define SPACE 0x20
#define BACKSPACE 0x08
#define LEFT 0x25
#define UP 0x26
#define RIGHT 0x27
#define DOWN 0x28

void tty_initialize();
void tty_clear();
void tty_putchar(char c);
void tty_buffer_clear();
char tty_getchar();
char* tty_gets();
