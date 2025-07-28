#include <kernel/pic.h>
#include <kernel/port.h>

void pic_remap(void) {
	write_port(PIC1_COMMAND, 0x11);
	write_port(PIC2_COMMAND, 0x11);

	write_port(PIC1_DATA, 0x20);
	write_port(PIC2_DATA, 0x28);

	write_port(PIC1_DATA, 0x4);
	write_port(PIC2_DATA, 0x2);

	write_port(PIC1_DATA, 0x01);
	write_port(PIC2_DATA, 0x01);

	write_port(PIC1_DATA, 0x00);
	write_port(PIC2_DATA, 0x00);
}