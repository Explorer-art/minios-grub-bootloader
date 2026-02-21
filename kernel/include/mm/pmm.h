#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <mm/mmu.h>

#define PMM_INTR_START_ADDR     0x400000
#define PMM_INTR_END_ADDR       0x7FFFFF
#define PMM_START_ADDR          0x800000
#define PMM_END_ADDR            0xFFFFFFFF

#define PMM_INTR_PAGES_COUNT     ((PMM_INTR_END_ADDR - PMM_INTR_START_ADDR) / PAGE_SIZE)

void pmm_init(void);
void* pmm_alloc_page(void);
void pmm_free_page(uint32_t page_addr);

extern page_directory_t kpage_directory[PAGE_DIRECTORY_ENTRIES_COUNT];

#endif
