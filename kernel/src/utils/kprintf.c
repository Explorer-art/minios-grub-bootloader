#include <utils/kprintf.h>
#include <drivers/tty.h>
#include <drivers/keyboard.h>
#include <stdbool.h>

static const char hex_chars[] = "0123456789abcdef";

// Делит 64-битное число (number) на 32-битное (divisor)
// Возвращает остаток, а частное записывает в *quotient
static uint32_t div64_32(uint64_t number, uint32_t divisor, uint64_t* quotient) {
    uint32_t remainder;
    uint32_t low = (uint32_t)(number & 0xFFFFFFFF);
    uint32_t high = (uint32_t)(number >> 32);
    uint32_t q_low;

    // Используем asm div: делит edx:eax на divisor
    // Частное — eax, остаток — edx
    __asm__ volatile (
        "divl %[divisor]"
        : "=a"(q_low), "=d"(remainder)
        : "a"(low), "d"(high), [divisor]"r"(divisor)
    );

    *quotient = ((uint64_t)0 << 32) | q_low; // Частное всегда помещаем в 64-бит
    return remainder;
}

int* printf_number(int* argp, int length, bool sign, int radix) {
    char buffer[32];
    unsigned long long number;
    int number_sign = 1;
    int pos = 0;

    // process length
    switch (length) {
        case PRINTF_LENGTH_SHORT_SHORT:
        case PRINTF_LENGTH_SHORT:
        case PRINTF_LENGTH_DEFAULT:
            if (sign)
            {
                int n = *argp;
                if (n < 0)
                {
                    n = -n;
                    number_sign = -1;
                }
                number = (unsigned long long)n;
            }
            else
            {
                number = *(unsigned int*)argp;
            }
            argp++;
            break;

        case PRINTF_LENGTH_LONG:
            if (sign)
            {
                long int n = *(long int*)argp;
                if (n < 0)
                {
                    n = -n;
                    number_sign = -1;
                }
                number = (unsigned long long)n;
            }
            else
            {
                number = *(unsigned long int*)argp;
            }
            argp += 2;
            break;

        case PRINTF_LENGTH_LONG_LONG:
            if (sign)
            {
                long long int n = *(long long int*)argp;
                if (n < 0)
                {
                    n = -n;
                    number_sign = -1;
                }
                number = (unsigned long long)n;
            }
            else
            {
                number = *(unsigned long long int*)argp;
            }
            argp += 4;
            break;
    }

    // convert number to ASCII
    do
    {
        uint32_t rem;
        rem = div64_32(number, radix, &number);
        buffer[pos++] = hex_chars[rem];
    } while (number > 0);

    // add sign
    if (sign && number_sign < 0)
        buffer[pos++] = '-';

    // print number in reverse order
    while (--pos >= 0)
        tty_putchar(buffer[pos]);

    return argp;
}

void kprintf(const char* fmt, ...) {
	int* argp = (int*)&fmt;
	int state = PRINTF_STATE_NORMAL;
	int length = PRINTF_LENGTH_DEFAULT;
	int radix = 10;
	bool sign = true;

	argp++;

	while (*fmt) {
		switch (state) {
			case PRINTF_STATE_NORMAL:
				switch (*fmt) {
				case '%':
					state = PRINTF_STATE_LENGTH;
					break;
				case '\n':
					tty_putchar('\n');
					break;
				default:
					tty_putchar(*fmt);
					break;
				}
				break;
			case PRINTF_STATE_LENGTH:
				switch (*fmt) {
					case 'h':
						state = PRINTF_STATE_LENGTH_SHORT;
						length = PRINTF_LENGTH_SHORT;
						break;
					case 'l':
						state = PRINTF_STATE_LENGTH_LONG;
						length = PRINTF_LENGTH_LONG;
						break;
					default:
						goto PRINTF_STATE_SPEC_;
				}
				break;
			case PRINTF_STATE_LENGTH_SHORT:
				if (*fmt == 'h') {
					state = PRINTF_STATE_SPEC;
					length = PRINTF_LENGTH_SHORT_SHORT;
				} else {
					goto PRINTF_STATE_SPEC_;
				}
				break;
			case PRINTF_STATE_LENGTH_LONG:
				if (*fmt == 'l') {
					state = PRINTF_STATE_SPEC;
					length = PRINTF_LENGTH_LONG_LONG;
				} else {
					goto PRINTF_STATE_SPEC_;
				}
				break;
			case PRINTF_STATE_SPEC:
				PRINTF_STATE_SPEC_:
				switch (*fmt) {
					case 'c':
						tty_putchar((char)*argp);
						argp++;
						break;
					case 's':
						if (length == PRINTF_LENGTH_LONG || length == PRINTF_LENGTH_LONG_LONG) {
							tty_puts(*(const char**)argp);
							argp++;
							break;
						} else {
							tty_puts(*(const char**)argp);
							argp++;
							break;
						}
					case '%':
						tty_putchar('%');
						break;
					case 'd':
					case 'i':
						radix = 10;
						sign = true;
						argp = printf_number(argp, length, sign, radix);
						break;
					case 'u':
						radix = 10;
						sign = false;
						argp = printf_number(argp, length, sign, radix);
						break;
					case 'X':
					case 'x':
					case 'p':
						radix = 16;
						sign = false;
						argp = printf_number(argp, length, sign, radix);
						break;
					case 'o':
						radix = 8;
						sign = false;
						argp = printf_number(argp, length, sign, radix);
						break;
					default:
						break;
				}

				state = PRINTF_STATE_NORMAL;
				length = PRINTF_LENGTH_DEFAULT;
				break;
		}

		fmt++;
	}
}
