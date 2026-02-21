#ifndef MMU_H
#define MMU_H

#include <stdint.h>

#define PAGE_SIZE       4096
#define PAGE_DIRECTORY_ENTRIES_COUNT 1024
#define PAGE_TABLE_ENTRIES_COUNT 1024
#define PAGES_COUNT     ((PMM_END_ADDR - PMM_START_ADDR) / PAGE_SIZE)

#define PGROUNDUP(sz)   (((sz)+((uintptr_t)PAGE_SIZE-1)) & ~((uintptr_t)(PAGE_SIZE - 1)))
#define PGROUNDDOWN(a)  (((uintptr_t)(a)) & ~((uintptr_t)(PAGE_SIZE - 1)))

#define PTXSHIFT        12
#define PDXSHIFT        22
#define PXMASK          0x3FF

// page directory index
#define PDX(va)         (((uintptr_t)(va) >> PDXSHIFT) & PXMASK)

// page table index
#define PTX(va)         (((uintptr_t)(va) >> PTXSHIFT) & PXMASK)

typedef struct {
	uint32_t present : 1;
	uint32_t read_write : 1;
	uint32_t user_supervisor : 1;
	uint32_t write_through : 1;
	uint32_t cache_disabled : 1;
	uint32_t accessed : 1;
	uint32_t zero : 1;
	uint32_t page_size : 1;
	uint32_t ignored : 1;
	uint32_t available : 3;
	uint32_t page_table_addr : 20;
} __attribute__((packed)) page_directory_t;

typedef struct {
	uint32_t present : 1;
	uint32_t read_write : 1;
	uint32_t user_supervisor : 1;
	uint32_t write_through : 1;
	uint32_t cache_disabled : 1;
	uint32_t accessed : 1;
	uint32_t dirty : 1;
	uint32_t zero : 1;
	uint32_t global : 1;
	uint32_t available : 3;
	uint32_t page_phys_addr : 20;
} __attribute__((packed)) page_table_t;

#endif