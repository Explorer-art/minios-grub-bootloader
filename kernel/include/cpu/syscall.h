#ifndef SYSCALL_H
#define SYSCALL_H

#include <cpu/irq.h>
#include <stdint.h>

#define SYSCALLS_SIZE 2

typedef uint8_t (*syscall_t)(registers_t*);

#endif
