#ifndef VMM_H
#define VMM_H

#include <stdint.h>

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
} __attribute__((packed)) Page_Directory_Entry;

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
} __attribute__((packed)) Page_Table_Entry;

void vmm_init(void);

#endif
