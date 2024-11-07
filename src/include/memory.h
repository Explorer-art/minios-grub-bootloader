#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

void* memset(void* ptr, int value, size_t size);
int memcmp(const void* ptr1, const void* ptr2, size_t size);
void* memcpy(void* __restrict dstptr, const void* __restrict srcptr, size_t size);
void* memmove(void* dstptr, const void* srcptr, size_t size);

#endif