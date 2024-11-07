#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct GDT_Entry {
	uint16_t limit;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t flags;
	uint8_t base_high;
} __attribute__((packed));

struct GDT_Ptr {
	uint16_t limit;
	unsigned int base;
} __attribute__((packed));

void GDT_init();
void GDT_set(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

#endif