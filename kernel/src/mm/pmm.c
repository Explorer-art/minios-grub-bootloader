#include <mm/pmm.h>
#include <utils/kpanic.h>
#include <stddef.h>
#include <memory.h>

static uint32_t bitmap[PAGES_COUNT / 32];

void pmm_init(void) {
    memset(bitmap, 0, sizeof(bitmap));
}

int find_first_page(void) {
    for (int i = 0; i < PAGES_COUNT / 32; i++) {
        if (bitmap[i] != 0xFFFFFFFF) {
            for (int j = 0; j < 32; j++) {
                if (((bitmap[i] >> j) & 1) == 0) {
                    return i * 32 + j;
                }
            }
        }
    }

    return -1;
}

void bitmap_set(uint32_t page_index) {
    uint32_t index = page_index / 32;
    uint32_t offset = page_index % 32;

    bitmap[index] |= 1U << offset;
}

void bitmap_clear(uint32_t page_index) {
    uint32_t index = page_index / 32;
    uint32_t offset = page_index % 32;

    bitmap[index] &= ~(1U << offset);
}

void* pmm_alloc_page(void) {
    int index = find_first_page();

    if (index == -1) {
        return NULL;
    }

    bitmap_set(index);

    return (void*)(index * PAGE_SIZE + PMM_START_ADDR);
}

void pmm_free_page(uint32_t page_addr) {
    if (page_addr < PMM_START_ADDR) return;

    uint32_t page_index = (page_addr - PMM_START_ADDR) / PAGE_SIZE;
    if (page_index >= PAGES_COUNT) return;

    bitmap_clear(page_index);
}
