#ifndef KPRINTF_H
#define KPRINTF_H

#include <stdint.h>

#define PRINTF_STATE_NORMAL 0
#define PRINTF_STATE_LENGTH 1
#define PRINTF_STATE_LENGTH_SHORT 2
#define PRINTF_STATE_LENGTH_LONG 3
#define PRINTF_STATE_SPEC 4

#define PRINTF_LENGTH_DEFAULT 0
#define PRINTF_LENGTH_SHORT 1
#define PRINTF_LENGTH_SHORT_SHORT 2
#define PRINTF_LENGTH_LONG 3
#define PRINTF_LENGTH_LONG_LONG 4

void kprintf(const char* fmt, ...);

#endif
