#include <cpu/gdt.h>
#include <memory.h>

extern void gdt_flush(uint32_t addr);

GDT_Entry gdt_entries[GDT_ENTRIES];
GDT_Ptr gdt_ptr;

void gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
	gdt_entries[num].base_low = base & 0xFFFF;
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high = (base >> 24) & 0xFF;
	gdt_entries[num].limit = limit & 0xFFFF;
	gdt_entries[num].access = access;
	gdt_entries[num].flags = (limit >> 16) & 0x0F;
	gdt_entries[num].flags |= flags & 0xF0;
}

void gdt_init(void) {
	gdt_ptr.limit = (sizeof(GDT_Entry) * GDT_ENTRIES) - 1;
	gdt_ptr.base = (uint32_t)&gdt_entries;

	gdt_set_gate(0, 0, 0, 0, 0); // Null descriptor
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel data segment
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User code segment
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User data segment

	gdt_flush((uint32_t)&gdt_ptr);
}
