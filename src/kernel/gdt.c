#include <kernel/gdt.h>
#include <memory.h>

extern void gdt_flush(unsigned int addr);
extern void tss_flush();

struct GDT_Entry gdt_entries[6];
struct GDT_Ptr gdt_ptr;
struct TSS_Entry tss_entry;

void set_gdt_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
	gdt_entries[num].base_low = base & 0xFFFF;
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high = (base >> 24) & 0xFF;
	gdt_entries[num].limit = limit & 0xFFFF;
	gdt_entries[num].access = access;
	gdt_entries[num].flags = (limit >> 16) & 0x0F;
	gdt_entries[num].flags |= flags & 0xF0;
}

void write_tss(uint32_t num, uint16_t ss0, uint16_t esp0) {
	uint32_t base = (uint32_t) &tss_entry;
	uint32_t limit = base + sizeof(tss_entry);

	set_gdt_gate(num, base, limit, 0xE9, 0);

	memset(&tss_entry, 0, sizeof(tss_entry));

	tss_entry.ss0 = ss0;
	tss_entry.esp0 = esp0;
	tss_entry.cs = 0x08 | 0x3;
	tss_entry.es = tss_entry.ss = tss_entry.ds = tss_entry.fs = tss_entry.gs = 0x10 | 0x3;
}

void gdt_init() {
	gdt_ptr.limit = (sizeof(struct GDT_Entry) * 6) - 1;
	gdt_ptr.base = (unsigned int) &gdt_entries;

	set_gdt_gate(0, 0, 0, 0, 0); // Null descriptor
	set_gdt_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code segment
	set_gdt_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel data segment
	set_gdt_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User code segment
	set_gdt_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User data segment
	write_tss(5, 0x10, 0);

	gdt_flush((unsigned int) &gdt_ptr);
	tss_flush();
}