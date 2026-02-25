#include <cpu/pic.h>
#include <x86.h>

void pic_remap(uint8_t offset1, uint8_t offset2) {
	outb(PIC1_COMMAND, 0x11);
	outb(PIC2_COMMAND, 0x11);

	outb(PIC1_DATA, offset1);
	outb(PIC2_DATA, offset2);

	outb(PIC1_DATA, 0x4);
	outb(PIC2_DATA, 0x2);

	outb(PIC1_DATA, 0x01);
	outb(PIC2_DATA, 0x01);

	outb(PIC1_DATA, 0x00);
	outb(PIC2_DATA, 0x00);
}