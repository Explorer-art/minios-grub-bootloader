#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#define GDT_ENTRIES 5

typedef struct {
	uint16_t limit;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t flags;
	uint8_t base_high;
} __attribute__((packed)) GDT_Entry;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) GDT_Ptr;

void gdt_init(void);

#endif