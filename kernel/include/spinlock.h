#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <stdint.h>

typedef struct {
    uint32_t locked;
    char* name;
    cpu_t* cpu;
    uint32_t pcs[10];
} spinlock_t;

#endif