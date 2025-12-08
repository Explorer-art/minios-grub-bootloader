#include <kernel/mm/vmm.h>
#include <kernel/mm/pmm.h>
#include <kernel/utils/kprintf.h>
#include <memory.h>

extern void paging_enable(uint32_t page_directory_addr);
extern void paging_disable(void);

static page_directory_t kpage_directory[PAGE_DIRECTORY_ENTRIES_COUNT] __attribute__((aligned(4096)));
static page_table_t kpage_table[PAGE_TABLE_ENTRIES_COUNT] __attribute__((aligned(4096)));

void vmm_init(void) {
    memset(kpage_directory, 0, sizeof(page_directory_t) * PAGE_DIRECTORY_ENTRIES_COUNT);

    // Fill kernel page directory
    for (int i = 0; i < PAGE_DIRECTORY_ENTRIES_COUNT; i++) {
        kpage_directory[i].zero = 1;
        kpage_directory[i].accessed = 0;
        kpage_directory[i].available = 0;
        kpage_directory[i].cache_disabled = 0;
        kpage_directory[i].ignored = 0;
        kpage_directory[i].page_size = 0;
        kpage_directory[i].present = 0; // No present
        kpage_directory[i].read_write = 1;
        kpage_directory[i].user_supervisor = 1;
        kpage_directory[i].write_through = 1;
    }

    kpage_directory[0].present = 1;
    kpage_directory[0].page_table_addr = (uint32_t)kpage_table >> 12;

    // 0xC0000000
    kpage_directory[768].present = 1;
    kpage_directory[768].page_table_addr = (uint32_t)kpage_table >> 12;

    // Fill kernel page table

    for (int i = 0; i < PAGE_TABLE_ENTRIES_COUNT; i++) {
        kpage_table[i].zero = 0;
        kpage_table[i].accessed = 0;
        kpage_table[i].available = 0;
        kpage_table[i].cache_disabled = 0;
        kpage_table[i].dirty = 0;
        kpage_table[i].global = 1;
        kpage_table[i].present = 1;
        kpage_table[i].read_write = 1;
        kpage_table[i].user_supervisor = 1;
        kpage_table[i].write_through = 1;
        kpage_table[i].page_phys_addr = (i * PAGE_SIZE) >> 12;
    }

    paging_enable(kpage_directory);
}

page_directory_t* vmm_create_user_page_directory(void) {
    page_directory_t* new_pd = pmm_alloc_page();

    if (new_pd == NULL) {
        kpanic("No free pages");
        return NULL;
    }

    // Clear page directory
    memset(new_pd, 0, sizeof(page_directory_t) * PAGE_DIRECTORY_ENTRIES_COUNT);

    // Copy kernel space mapping
    new_pd[768].present = 1;
    new_pd[768].user_supervisor = 0;
    new_pd[768].page_table_addr = (uint32_t)kpage_table >> 12;

    return new_pd;
}

page_table_t* vmm_create_user_page_table() {
    page_table_t* new_pt = pmm_alloc_page();

    if (new_pt == NULL) {
        kpanic("No free pages");
        return NULL;
    }

    // Clear page table
    memset(new_pt, 0, sizeof(page_directory_t) * PAGE_TABLE_ENTRIES_COUNT);

    for (int i = 0; i < PAGE_TABLE_ENTRIES_COUNT; i++) {
        new_pt[i].zero = 0;
        new_pt[i].accessed = 0;
        new_pt[i].available = 0;
        new_pt[i].cache_disabled = 0;
        new_pt[i].dirty = 0;
        new_pt[i].global = 1;
        new_pt[i].present = 1;
        new_pt[i].read_write = 1;
        new_pt[i].user_supervisor = 1;
        new_pt[i].write_through = 1;
        new_pt[i].page_phys_addr = (i * PAGE_SIZE) >> 12;
    }

    return new_pt;
}

void vmm_destroy_user_page_directory(page_directory_t* page) {
    pmm_free_page(page);
}
