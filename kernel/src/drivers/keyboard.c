#include <drivers/keyboard.h>
#include <cpu/irq.h>
#include <cpu/port.h>
#include <drivers/tty.h>
#include <utils/kprintf.h>

/* The following array is taken from
    http://www.osdever.net/bkerndev/Docs/keyboard.htm
   All credits where due
*/

const uint8_t lowercase[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
    '9', '0', '-', '=', '\b',	/* Backspace */
    '\t',			/* Tab */
    'q', 'w', 'e', 'r',	/* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
    '\'', '`',   0,		/* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
    'm', ',', '.', '/',   0,				/* Right shift */
    '*',
    0,	/* Alt */
    ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
    '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
    '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

const uint8_t uppercase[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', '-', '=', '\b',   /* Backspace */
    '\t',           /* Tab */
    'Q', 'W', 'E', 'R', /* 19 */
    'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',   /* Enter key */
    0,          /* 29   - Control */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',   /* 39 */
    '\'', '`',   0,     /* Left shift */
    '\\', 'Z', 'X', 'C', 'V', 'B', 'N',         /* 49 */
    'M', ',', '.', '/',   0,                /* Right shift */
    '*',
    0,  /* Alt */
    ' ',    /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

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
        c = uppercase[key];
    else
        c = lowercase[key];

    keyboard_buffer[write_index] = c;
    write_index = (write_index + 1) & (KEYBOARD_BUFFER_SIZE - 1);
}

uint8_t keyboard_getchar(void) {
    __asm__ volatile ("sti");
    while (read_index == write_index) {
        // Wait key pressed...
    }

    uint8_t c = keyboard_buffer[read_index];
    read_index = (read_index + 1) & (KEYBOARD_BUFFER_SIZE - 1);

    tty_putchar(c);

    __asm__ volatile ("cli");

    return c;
}

void keyboard_init(void) {
	irq_install_handler(1, keyboard_handler);
}
