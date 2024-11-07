#include <kernel/dynamic_memory.h>
#include <stdio.h>
#include <stdint.h>
#include <memdefs.h>

#define MEMORY_MAX_HANDLES 100

static MemoryData memory_table[MEMORY_MAX_HANDLES];
static int count = 0;

static uint32_t far* MEMORY_FREE_START = (uint32_t far*) MEMORY_FREE_ADDR;

bool check_range(uint32_t number, uint32_t range_start, uint32_t range_end) {
	if (number >= range_start && number <= range_end) {
		return true;
	}

	return false;
}

void print_memory_table() {
	for (int i = 0; i < count; i++) {
		printf("Index: %d\n", i);
	}
}

void* mem_allocate(uint32_t size) {
	printf("\r%d\n", count);

	if (size > MEMORY_FREE_SIZE || count >= MEMORY_MAX_HANDLES) {
		return NULL;
	}

	if (count == 0) {
		memory_table[count].Start = MEMORY_FREE_START;
		memory_table[count].Size = size;
		count++;

		return memory_table[count - 1].Start;
	}

	bool check;

	for (uint32_t address = MEMORY_FREE_START; address < 0x00010500; address++) {
		check = true;

		for (int i = 0; i < count; i++) {
			if (!check_range(address, memory_table[i].Start, memory_table[i].Start + memory_table[i].Size - 1)) {
				check = true;
			} else {
				check = false;
				break;
			}
		}

		if (check) {
			for (int i = 0; i < count; i++) {
				if (!check_range(address + size, memory_table[i].Start, memory_table[i].Start + memory_table[i].Size - 1)) {
					check = true;
				} else {
					check = false;
					break;
				}
			}
		}

		if (check) {
			memory_table[count].Start = address;
			memory_table[count].Size = size;
			count++;

			return memory_table[count - 1].Start;
		}
	}

	return NULL;
}

void mem_free(void* ptr) {
	uint8_t memory_index = -1;

	for (int i = 0; i < count; i++) {
		if (memory_table[i].Start == (uint32_t) ptr) {
			memory_index = i;
		}
	}

	if (memory_index == -1) {
		return;
	}

	for (int i = memory_index; i < count - 1; i++) {
		memory_table[i] = memory_table[i + 1];
	}

	count--;
}
