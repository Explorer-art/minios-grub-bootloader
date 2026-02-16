#include <ctype.h>

int tolower(int c) {
    if (c < 'A' || c > 'z')
        return c;

    if (c < 'a' || c > 'z')
        return c;

    return c + 32;
}

int toupper(int c) {
    if (c < 'A' || c > 'z')
        return c;

    if (c < 'a' || c > 'z')
        return c;

    return c - 32;
}