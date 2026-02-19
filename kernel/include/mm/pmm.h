#ifndef PMM_H
#define PMM_H

#include <stdint.h>

#define PMM_START_ADDR  0x400000
#define PMM_END_ADDR    0xFFFFFFFF
#define PAGE_SIZE 4096
#define PAGES_COUNT ((PMM_END_ADDR - PMM_START_ADDR) / PAGE_SIZE)

void pmm_init(void);
void* pmm_alloc_page(void);
void pmm_free_page(uint32_t page_addr);

#endif
