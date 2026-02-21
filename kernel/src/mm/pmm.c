#include <mm/pmm.h>
#include <utils/kpanic.h>
#include <stddef.h>
#include <memory.h>

static uint32_t bitmap[PAGES_COUNT / 32];
static uint32_t intr_bitmap[PMM_INTR_PAGES_COUNT / 32];

void pmm_init(void) {
    memset(bitmap, 0, sizeof(bitmap));
    memset(intr_bitmap, 0, sizeof(intr_bitmap));
}

int find_first_page(uint32_t* bitmap, int count) {
    for (int i = 0; i < count / 32; i++) {
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

void bitmap_set(uint32_t* bitmap, uint32_t page_index) {
    uint32_t index = page_index / 32;
    uint32_t offset = page_index % 32;

    bitmap[index] |= 1U << offset;
}

void bitmap_clear(uint32_t* bitmap, uint32_t page_index) {
    uint32_t index = page_index / 32;
    uint32_t offset = page_index % 32;

    bitmap[index] &= ~(1U << offset);
}

void* pmm_intr_alloc_page(void) {
    int index = find_first_page(intr_bitmap, PMM_INTR_PAGES_COUNT);

    if (index == -1) return NULL;

    bitmap_set(intr_bitmap, index);

    return (void*)(index * PAGE_SIZE + PMM_INTR_START_ADDR);
}

void pmm_intr_free_page(uint32_t page_addr) {
    if (page_addr < PMM_INTR_START_ADDR) return;

    uint32_t page_index = (page_addr - PMM_INTR_START_ADDR) / PAGE_SIZE;
    if (page_index >= PAGES_COUNT) return;

    bitmap_clear(intr_bitmap, page_index);
}

void* pmm_alloc_page(void) {
    int index = find_first_page(bitmap, PAGES_COUNT);

    if (index == -1) return NULL;

    bitmap_set(bitmap, index);

    int page_addr = (index * PAGE_SIZE + PMM_START_ADDR);
    page_table_t* ptable;
    int pd = PDX(page_addr);
    int pt = PTX(page_addr);

    if (!kpage_directory[pd].present) {
        if ((ptable = pmm_intr_alloc_page()) == NULL) {
            kpanic("Failed allocate page table");
        }

        kpage_directory[pd].present = 1;
        kpage_directory[pd].page_table_addr = (uint32_t)ptable >> 12;
    } else {
        ptable = (page_table_t*)(((uint32_t)kpage_directory[pd].page_table_addr << 12) + (pt * sizeof(page_table_t*)));
    }

    if (!ptable->present) {
        ptable->zero = 0;
        ptable->accessed = 0;
        ptable->available = 0;
        ptable->cache_disabled = 0;
        ptable->dirty = 0;
        ptable->global = 1;
        ptable->present = 1;
        ptable->read_write = 1;
        ptable->user_supervisor = 0;
        ptable->write_through = 1;
        ptable->page_phys_addr = page_addr >> 12;
    } else {
        kpanic("Page already allocated");
    }

    return (void*)page_addr;
}

void pmm_free_page(uint32_t page_addr) {
    if (page_addr < PMM_START_ADDR) return;

    uint32_t page_index = (page_addr - PMM_START_ADDR) / PAGE_SIZE;
    if (page_index >= PAGES_COUNT) return;

    bitmap_clear(bitmap, page_index);
}
