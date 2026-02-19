#include <stdint.h>
#include <stddef.h>

#define KMALLOC_START_ADDR      0x300000
#define KMALLOC_END_ADDR        0x3FFFFF
#define KMALLOC_SIZE            (KMALLOC_END_ADDR - KMALLOC_START_ADDR + 1)
#define KMALLOC_MIN_BLOCK_SIZE  2

typedef struct {
    uint16_t size;
    uint16_t flags;
} __attribute__((packed)) kmalloc_block_info_t;

void kmalloc_init(void);
void* kmalloc(uint32_t size);
void* kcalloc(size_t number, size_t size);
void kfree(uint8_t* ptr);