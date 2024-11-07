#include <kernel/gdt.h>
#include <stdint.h>

extern void gdt_flush(unsigned int addr);

struct GDT_Entry gdt_entries[5];
struct GDT_Ptr gdt_ptr;

void set_gdt_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	gdt_entries[num].base_low = base & 0xFFFF;
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high = (base >> 24) & 0xFF;
	gdt_entries[num].limit = limit & 0xFFFF;
	gdt_entries[num].access = access;
	gdt_entries[num].flags = (limit >> 16) & 0x0F;
	gdt_entries[num].flags |= gran & 0xF0;
}

void GDT_init() {
	gdt_ptr.limit = (sizeof(struct GDT_Entry) * 5) - 1;
	gdt_ptr.base = (unsigned int) &gdt_entries;

	set_gdt_gate(0, 0, 0, 0, 0); // Null descriptor
	set_gdt_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code segment
	set_gdt_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel data segment
	set_gdt_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User code segment
	set_gdt_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User data segment

	gdt_flush((unsigned int) &gdt_ptr);
}