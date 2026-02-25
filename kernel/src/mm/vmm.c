#include <mm/vmm.h>
#include <mm/pmm.h>
#include <x86.h>
#include <utils/kprintf.h>
#include <memory.h>

extern process_t* current_process;

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

    lcr3(kpage_directory);
    paging_enable();
}

page_directory_t* vmm_create_user_page_directory(void) {
    page_directory_t* pgdir = pmm_intr_alloc_page();

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

    pgdir[0].present = 1;
    pgdir[0].page_table_addr = (uint32_t)kpage_table1 >> 12;
    pgdir[1].present = 1;
    pgdir[1].page_table_addr = (uint32_t)kpage_table2 >> 12;

    return pgdir;
}

void* vmm_va2pa(page_directory_t* pgdir, uint8_t* va) {
    page_table_t* ptable;
    int pd = PDX(va);
    int pt = PTX(va);

    if (!pgdir[pd].present) return NULL;

    ptable = (page_table_t*)((uint32_t)pgdir[pd].page_table_addr << 12);

    if (!ptable[pt].present) return NULL;

    uint32_t pa = (ptable[pt].page_phys_addr << 12) | ((uint32_t)va & 0xFFF);

    return (void*)pa;
}

page_table_t* vmm_walk_pgdir(page_directory_t* pgdir, uint8_t* va) {
    page_table_t* ptable;
    int pd = PDX(va);
    int pt = PTX(va);

    if (!pgdir[pd].present) {
        ptable = pmm_intr_alloc_page();

        if (ptable == NULL) return NULL;

        memset(ptable, 0, PAGE_SIZE);

        pgdir[pd].present = 1;
        pgdir[pd].page_table_addr = (uint32_t)ptable >> 12;
    } else {
        ptable = (page_table_t*)((uint32_t)pgdir[pd].page_table_addr << 12);
    }

    return &ptable[pt];
}

int vmm_mappage(page_directory_t* pgdir, uint8_t* va, uint8_t* pa, uint32_t size, uint8_t pte_p, uint8_t pte_w, uint8_t pte_u) {
    kprintf("Mappage: VA=%x, PA=%x\n", va, pa);
    
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

void vmm_switch_kernel_page_directory(void) {
    lcr3(kpage_directory);
}

void vmm_switch_user_page_directory(page_directory_t* pgdir) {
    lcr3(pgdir);
}

void vmm_destroy_user_page_directory(page_directory_t* pgdir) {
    for (int i = 0; i < PAGE_DIRECTORY_ENTRIES_COUNT; i++) {
        kprintf("%d\n", pgdir[i].present);
        if (pgdir[i].present) {
            page_table_t* ptable = (page_table_t*)(pgdir[i].page_table_addr >> 12);

            for (int j = 0; j < PAGE_TABLE_ENTRIES_COUNT; j++) {
                if (ptable[j].present) {
                    pmm_free_page((ptable[j].page_phys_addr >> 12));
                }
            }
        }
    }
    pmm_free_page(pgdir);
}
