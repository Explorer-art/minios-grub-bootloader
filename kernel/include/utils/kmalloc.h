#include <stdint.h>
#include <stddef.h>

void* kmalloc(uint32_t size);
void* kcalloc(size_t number, size_t size);
void kfree(uint32_t* ptr);