#include <kernel/keyboard.h>
#include <kernel/tty.h>
#include <kernel/port.h>
#include <kernel/keyboard_map.h>

void keyboard_handler(void) {
	unsigned char status;
	char keycode;

	/* Пишем EOI */
	write_port(0x20, 0x20);

	status = read_port(KEYBOARD_STATUS_PORT);
	
	/* Нижний бит статуса будет выставлен, если буфер не пуст */
	if (status & 0x01) {
		keycode = read_port(KEYBOARD_DATA_PORT);
		if(keycode < 0)
			return;

		tty_putchar(keyboard_map[keycode]);
	}
}