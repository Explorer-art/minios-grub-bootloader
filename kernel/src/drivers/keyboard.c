#include <drivers/keyboard.h>
#include <cpu/irq.h>
#include <x86.h>
#include <drivers/tty.h>
#include <utils/kprintf.h>

static uint8_t caps = 0;
static uint8_t capslock = 0;
static uint8_t keyboard_buffer[1024];
static uint8_t write_index = 0;
static uint8_t read_index = 0;

void keyboard_handler(registers_t* regs) {
	uint8_t status = inb(0x64);

    /* Нижний бит статуса будет выставлен, если буфер не пуст */
    if (!(status & 0x01))
        return;

    uint8_t keycode = inb(0x60);
    uint8_t release = keycode & 0x80;
    uint8_t key = keycode & 0x7F;

    switch (key) {
        case KEY_CAPSLOCK:
            if (!release)
                capslock = !capslock;
            return;
        case KEY_LSHIFT:
        case KEY_RSHIFT:
            if (!release)
                caps = 1;
            else
                caps = 0;
            return;
	}

    if (release)
        return;

    char c;

    if ((caps ^ capslock) != 0)
        c = keyboard_uppercase[key];
    else
        c = keyboard_lowercase[key];

    keyboard_buffer[write_index] = c;
    write_index = (write_index + 1) & (KEYBOARD_BUFFER_SIZE - 1);
}

uint8_t keyboard_getchar(void) {
    sti();
    while (read_index == write_index) {
        // Wait key pressed...
    }

    uint8_t c = keyboard_buffer[read_index];
    read_index = (read_index + 1) & (KEYBOARD_BUFFER_SIZE - 1);

    tty_putchar(c);
    cli();

    return c;
}

void keyboard_init(void) {
	irq_install_handler(1, keyboard_handler);
}
