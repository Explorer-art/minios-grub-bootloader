#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <kernel/cpu/irq.h>
#include <stdint.h>

#define SYSCALLS_SIZE 2

typedef uint8_t (*syscall_t)(registers_t*);

#endif