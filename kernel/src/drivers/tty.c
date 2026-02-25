#include <drivers/tty.h>
#include <vga.h>
#include <x86.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static uint16_t* const VGA_MEMORY = (uint16_t* const) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void tty_setcolor(uint8_t color) {
	terminal_color = color;
}

void tty_cursor_enable(uint8_t cursor_start, uint8_t cursor_end) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void tty_cursor_disable(void) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

uint16_t tty_cursor_get_pos(void) {
	uint16_t pos = 0;

	outb(0x3D4, 0x0F);
	pos |= inb(0x3D5);
	outb(0x3D4, 0x0E);
	pos |= ((uint16_t)inb(0x3D5)) << 8;

	return pos;
}

void tty_cursor_update(uint8_t x, uint8_t y) {
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void tty_init(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}

	tty_cursor_disable();
}

void tty_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
	tty_cursor_update(x + 1, y);
}

void tty_clear(void) {
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void tty_scroll(size_t line) {
	uint16_t loop;
	uint16_t c;

	if (line < 1 || line >= VGA_HEIGHT) {
		return;
	}

	for (loop = line * VGA_WIDTH; loop < VGA_HEIGHT * VGA_WIDTH; loop++) {
		c = terminal_buffer[loop];
		terminal_buffer[loop - VGA_WIDTH] = c;
	}

	for (loop = (VGA_HEIGHT - line) * VGA_WIDTH; loop < VGA_HEIGHT * VGA_WIDTH; loop++) {
		terminal_buffer[loop] = vga_entry(' ', terminal_color);
	}
}

void tty_new_line() {
	terminal_column = 0;
	terminal_row++;

	if (terminal_row >= VGA_HEIGHT) {
		terminal_row = VGA_HEIGHT - 1;
		tty_scroll(1);
	}

	tty_cursor_update(terminal_column, terminal_row);
}

void tty_putchar(char c) {
	unsigned char uc = c;

	if (c == '\n') {
		tty_new_line();
	} else {
		tty_putentryat(uc, terminal_color, terminal_column, terminal_row);

		if (++terminal_column == VGA_WIDTH) {
			tty_new_line();
		}
	}
}

void tty_puts(const char* str) {
	for (size_t i = 0; i < strlen(str); i++) {
		tty_putchar(str[i]);
	}
}