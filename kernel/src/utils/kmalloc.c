/*-------------------------------------------------
    
Simple kernel memory allocator

Structure:
|   2   |   2   |
| SIZE  | FLAGS |

Memory example:
| 8 | memory (8 bytes) | 4 | memory (4 bytes) |

-------------------------------------------------*/



#include <utils/kmalloc.h>
#include <utils/kpanic.h>
#include <memory.h>

void* find_free_block(size_t size) {
    uint32_t i = 0;
    
    while (i < KMALLOC_SIZE) {
        if (KMALLOC_START_ADDR + i + sizeof(kmalloc_block_info_t) + size > KMALLOC_START_ADDR + KMALLOC_SIZE) return NULL;

        kmalloc_block_info_t* info = (kmalloc_block_info_t*)(uint8_t*)(KMALLOC_START_ADDR + i);
        if (!(info->flags & 1)) {
            if (info->size >= size || info->size == 0) {
                return KMALLOC_START_ADDR + i;
            }
        }

        i += sizeof(kmalloc_block_info_t) + info->size;
    }

    return NULL;
}

void* alloc_block(size_t size) {
    if (!size) return NULL;

    uint32_t* addr = find_free_block(size);

    if (!addr) {
        kpanic("Out of memory for kernel allocate");
    }

    kmalloc_block_info_t* info = (kmalloc_block_info_t*)addr;

    info->flags |= 1;

    if (info->size == 0) {
        info->size = size;
    } else if (info->size < size + sizeof(kmalloc_block_info_t) + KMALLOC_MIN_BLOCK_SIZE) {
        info->size = size + sizeof(kmalloc_block_info_t) + KMALLOC_MIN_BLOCK_SIZE;
    } else if (info->size >= size + sizeof(kmalloc_block_info_t) + KMALLOC_MIN_BLOCK_SIZE) {
        kmalloc_block_info_t* new_info = (kmalloc_block_info_t*)(uint8_t*)(info + info->size);
        new_info->size = info->size - size - sizeof(kmalloc_block_info_t);
        info->size = size;
    }

    return (void*)addr + sizeof(kmalloc_block_info_t);
}

void kmalloc_init(void) {
    memset(KMALLOC_START_ADDR, 0, KMALLOC_SIZE);
}

void* kmalloc(uint32_t size) {
    return alloc_block(size);
}

void* kcalloc(size_t number, size_t size) {
    void* ptr = alloc_block(number * size);

    if (ptr) {
        memset(ptr, 0, number * size);
    }

    return ptr;
}

void kfree(uint8_t* ptr) {
    if (!ptr || ptr < KMALLOC_START_ADDR || ptr > KMALLOC_END_ADDR) return;

    kmalloc_block_info_t* info = (kmalloc_block_info_t*)(ptr - sizeof(kmalloc_block_info_t));
    info->flags &= ~1;
}
