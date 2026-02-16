#include <utils/kmalloc.h>
#include <mm/pmm.h>

void* kmalloc(uint32_t size) {
    return pmm_alloc_page();
}

void* kcalloc(size_t number, size_t size) {
    return pmm_alloc_page();
}

void kfree(uint32_t* ptr) {
    pmm_free_page((uint32_t)ptr);
}