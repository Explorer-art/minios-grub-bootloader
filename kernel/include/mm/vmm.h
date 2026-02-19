#ifndef VMM_H
#define VMM_H

#include <stdint.h>

#define PAGE_DIRECTORY_ENTRIES_COUNT 1024
#define PAGE_TABLE_ENTRIES_COUNT 1024

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

void vmm_init(void);
page_directory_t* vmm_create_user_page_directory(void);
void vmm_destroy_user_page_directory(page_directory_t* page);

#endif
