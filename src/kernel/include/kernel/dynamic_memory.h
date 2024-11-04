#pragma once

#include <stdint.h>

typedef struct {
	uint32_t Start;
	uint32_t Size;
} MemoryData;

void* mem_allocate(uint32_t size);
void mem_free(void *ptr);
void print_memory_table();
