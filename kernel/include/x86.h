#ifndef X86_H
#define X86_H

#include <process.h>
#include <mm/mmu.h>
#include <stdint.h>

static inline void cli(void) {
  asm volatile("cli");
}

static inline void sti(void) {
  asm volatile("sti");
}

static inline void hlt(void) {
  asm volatile("hlt");
}

static inline void lcr3(uintptr_t val) {
  asm volatile("mov %0,%%cr3" : : "r" (val));
}

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);
uint16_t inw(uint16_t port);
void outw(uint16_t port, uint16_t value);
void gdt_flush(uint32_t addr);
void idt_flush(uint32_t addr);
void paging_enable(void);
void paging_disable(void);
void switch_context(process_context_t** old_context, process_context_t* new_context);
void switch_kpgdir(page_directory_t* kpage, uint32_t esp);

#endif