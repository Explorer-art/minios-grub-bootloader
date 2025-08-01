#ifndef TTY_H
#define TTY_H

#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void tty_init(void);
void tty_setcolor(uint8_t color);
void tty_cursor_enable(uint8_t cursor_start, uint8_t cursor_end);
void tty_cursor_disable(void);
uint16_t tty_cursor_get_pos(void);
void tty_cursor_update(uint8_t x, uint8_t y);
void tty_clear(void);
void tty_putchar(char c);
void tty_puts(const char* str);

#endif