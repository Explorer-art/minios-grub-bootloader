#ifndef SYSCALL_H
#define SYSCALL_H

#include <cpu/irq.h>
#include <stdint.h>

#define SYSCALL_COUNT 3

typedef uint8_t (*syscall_t)(registers_t*);

#endif
