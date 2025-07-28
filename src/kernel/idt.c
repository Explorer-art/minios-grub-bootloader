#include <kernel/idt.h>
#include <kernel/port.h>
#include <memory.h>

// extern void keyboard_handler(void);
extern void idt_flush(uint32_t addr);

IDT_Entry idt_entries[IDT_SIZE];

void idt_set_gate(uint16_t num, uint32_t handler, uint16_t selector, uint8_t flags) {
	idt_entries[num].offset_low = handler & 0xFFFF;
	idt_entries[num].selector = selector;
	idt_entries[num].zero = 0;
	idt_entries[num].flags = flags;
	idt_entries[num].offset_high = (handler >> 16) & 0xFFFF;
}

void idt_init(void) {
	IDT_Ptr idt_ptr;

	idt_ptr.limit = sizeof(IDT_Entry) * IDT_SIZE - 1;
	idt_ptr.base = (uint32_t)&idt_entries;

	memset(&idt_entries, 0, sizeof(IDT_Entry) * 256);

	idt_set_gate(0x00, (uint32_t)isr0, 0x08, 0x8E); // Divide by zero

	idt_flush((uint32_t)&idt_ptr);
}