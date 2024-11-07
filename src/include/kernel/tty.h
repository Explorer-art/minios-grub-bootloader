#ifndef KERNEL_TTY_H
#define KERNEL_TTY_H

#include <stddef.h>

void terminal_initialize(void);
void terminal_clear(void);
void terminal_new_line(void);
void terminal_putchar(char c);
void terminal_write(const char* str, size_t size);
void terminal_writestring(const char* str);

#endif