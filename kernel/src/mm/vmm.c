#include <mm/vmm.h>
#include <mm/pmm.h>
#include <utils/kprintf.h>
#include <memory.h>

extern void paging_enable(uint32_t page_directory_addr);
extern void paging_disable(void);

page_directory_t kpage_directory[PAGE_DIRECTORY_ENTRIES_COUNT] __attribute__((aligned(4096)));
page_table_t kpage_table1[PAGE_TABLE_ENTRIES_COUNT] __attribute__((aligned(4096)));
page_table_t kpage_table2[PAGE_TABLE_ENTRIES_COUNT] __attribute__((aligned(4096)));

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
    kpage_directory[0].page_table_addr = (uint32_t)kpage_table1 >> 12;
    kpage_directory[1].present = 1;
    kpage_directory[1].page_table_addr = (uint32_t)kpage_table2 >> 12;

    // Fill kernel page table
    for (int i = 0; i < PAGE_TABLE_ENTRIES_COUNT; i++) {
        kpage_table1[i].zero = 0;
        kpage_table1[i].accessed = 0;
        kpage_table1[i].available = 0;
        kpage_table1[i].cache_disabled = 0;
        kpage_table1[i].dirty = 0;
        kpage_table1[i].global = 1;
        kpage_table1[i].present = 1;
        kpage_table1[i].read_write = 1;
        kpage_table1[i].user_supervisor = 1;
        kpage_table1[i].write_through = 1;
        kpage_table1[i].page_phys_addr = (i * PAGE_SIZE) >> 12;
    }

    for (int i = 0; i < PAGE_TABLE_ENTRIES_COUNT; i++) {
        kpage_table2[i].zero = 0;
        kpage_table2[i].accessed = 0;
        kpage_table2[i].available = 0;
        kpage_table2[i].cache_disabled = 0;
        kpage_table2[i].dirty = 0;
        kpage_table2[i].global = 1;
        kpage_table2[i].present = 1;
        kpage_table2[i].read_write = 1;
        kpage_table2[i].user_supervisor = 1;
        kpage_table2[i].write_through = 1;
        kpage_table2[i].page_phys_addr = (i * PAGE_SIZE + 0x400000) >> 12;
    }

    paging_enable(kpage_directory);
}

page_directory_t* vmm_create_user_page_directory(void) {
    page_directory_t* pgdir = pmm_alloc_page();

    if (pgdir == NULL) return NULL;

    page_table_t* ptable = (page_table_t*)(kpage_directory[2].page_table_addr << 12);

    for (int i = 0; i < PAGE_DIRECTORY_ENTRIES_COUNT; i++) {
        pgdir[i].zero = 1;
        pgdir[i].accessed = 0;
        pgdir[i].available = 0;
        pgdir[i].cache_disabled = 0;
        pgdir[i].ignored = 0;
        pgdir[i].page_size = 0;
        pgdir[i].present = 0; // No present
        pgdir[i].read_write = 1;
        pgdir[i].user_supervisor = 0;
        pgdir[i].write_through = 1;
    }

    return pgdir;
}

page_table_t* vmm_walk_pgdir(page_directory_t* pgdir, uint8_t* va) {
    page_table_t* ptable;

    if (!pgdir[PDX(va)].present) {
        ptable = pmm_alloc_page();

        if (ptable == NULL) return NULL;

        memset(ptable, 0, PAGE_SIZE);

        pgdir[PDX(va)].present = 1;
        pgdir[PDX(va)].page_table_addr = (uint32_t)ptable >> 12;
    } else {
        ptable = (page_table_t*)((uint32_t)pgdir[PDX(va)].page_table_addr << 12);
    }

    return &ptable[PTX(va)];
}

int vmm_mappage(page_directory_t* pgdir, uint8_t* va, uint8_t* pa, uint32_t size, uint8_t pte_p, uint8_t pte_w, uint8_t pte_u) {
    uint8_t *a, *last;

    a = PGROUNDDOWN(va);
    last = PGROUNDDOWN(va + size - 1);

    while (a <= last) {
        page_table_t* ptable = vmm_walk_pgdir(pgdir, a);
        
        if (!ptable) return -1;

        uint32_t pa_page = ((uint32_t)pa) + (a - PGROUNDDOWN(va));

        ptable->zero = 0;
        ptable->accessed = 0;
        ptable->available = 0;
        ptable->cache_disabled = 0;
        ptable->dirty = 0;
        ptable->global = 1;
        ptable->present = pte_p;
        ptable->read_write = pte_w;
        ptable->user_supervisor = pte_u;
        ptable->write_through = 1;
        ptable->page_phys_addr = pa_page >> 12;

        a += PAGE_SIZE;
    }

    return 0;
}

void vmm_destroy_user_page_directory(page_directory_t* pdir) {
    pmm_free_page(pdir);
}
