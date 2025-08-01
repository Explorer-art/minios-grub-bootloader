#include <kernel/pic.h>
#include <kernel/port.h>

void pic_remap(uint8_t offset1, uint8_t offset2) {
	write_port(PIC1_COMMAND, 0x11);
	write_port(PIC2_COMMAND, 0x11);

	write_port(PIC1_DATA, offset1);
	write_port(PIC2_DATA, offset2);

	write_port(PIC1_DATA, 0x4);
	write_port(PIC2_DATA, 0x2);

	write_port(PIC1_DATA, 0x01);
	write_port(PIC2_DATA, 0x01);

	write_port(PIC1_DATA, 0x00);
	write_port(PIC2_DATA, 0x00);
}