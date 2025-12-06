#include <kernel/mm/vmm.h>
#include <kernel/mm/pmm.h>
#include <kernel/utils/kprintf.h>
#include <memory.h>

extern void paging_enable(uint32_t page_directory_addr);
extern void paging_disable(void);

static Page_Directory_Entry page_directory[PAGE_DIRECTORY_ENTRIES_COUNT] __attribute__((aligned(4096)));

void vmm_init(void) {
    memset(&page_directory, 0, sizeof(Page_Directory_Entry) * PAGE_DIRECTORY_ENTRIES_COUNT);

    for (int i = 0; i < PAGE_DIRECTORY_ENTRIES_COUNT; i++) {
        Page_Table_Entry* page_table = pmm_alloc_page();

        page_directory[i].zero = 1;
        page_directory[i].accessed = 0;
        page_directory[i].available = 0;
        page_directory[i].cache_disabled = 0;
        page_directory[i].ignored = 0;
        page_directory[i].page_size = 0;
        page_directory[i].present = 1;
        page_directory[i].read_write = 1;
        page_directory[i].user_supervisor = 1;
        page_directory[i].write_through = 1;
        page_directory[i].page_table_addr = (uint32_t)page_table >> 12;

        for (int i = 0; i < PAGE_TABLE_ENTRIES_COUNT; i++) {
            page_table[i].zero = 0;
            page_table[i].accessed = 0;
            page_table[i].available = 0;
            page_table[i].cache_disabled = 0;
            page_table[i].dirty = 0;
            page_table[i].global = 1;
            page_table[i].present = 1;
            page_table[i].read_write = 1;
            page_table[i].user_supervisor = 1;
            page_table[i].write_through = 1;
            page_table[i].page_phys_addr = (i * PAGE_SIZE) >> 12;
        }
    }

    paging_enable(page_directory);
}
