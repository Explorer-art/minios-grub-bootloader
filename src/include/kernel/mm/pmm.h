#ifndef PMM_H
#define PMM_H

#include <stdint.h>

#define PAGE_SIZE 4096
#define PMM_START_ADDR 0x400000
#define PAGE_DIRECTORY_ENTRIES_COUNT 16
#define PAGE_TABLE_ENTRIES_COUNT 1024
#define PAGES_COUNT PAGE_DIRECTORY_ENTRIES_COUNT * PAGE_TABLE_ENTRIES_COUNT

void* pmm_alloc_page(void);
void pmm_free_page(uint32_t page_addr);

#endif
