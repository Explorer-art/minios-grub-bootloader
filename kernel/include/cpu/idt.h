#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_SIZE 256

typedef struct {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t zero;
	uint8_t flags;
	uint16_t offset_high;
} __attribute__((packed)) IDT_Entry;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) IDT_Ptr;

void idt_init(void);

extern void isr0();
extern void irq1();
extern void isr128();

#endif