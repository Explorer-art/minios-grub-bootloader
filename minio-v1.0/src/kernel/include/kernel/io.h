#pragma once

#include <stdint.h>

void _cdecl clear_screen();
void _cdecl write_char(char c);
char _cdecl read_char();
void _cdecl div64_32(uint64_t dividend, uint32_t divisor, uint64_t* quotient_out, uint32_t* remainder_out);
