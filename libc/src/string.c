#include <string.h>
#include <ctype.h>

size_t strlen(const char* str) {
	size_t len = 0;

	while (str[len]) {
		len++;
	}

	return len;
}

char* strcpy(char* dst, const char* src) {
	while (*src) {
		*dst = *src;
		src++;
		dst++;
	}

	*dst = '\0';
	return dst - 1;
}

char* strncpy(char* dst, const char* src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; ++i) {
        dst[i] = src[i];
    }
    
    for (; i < n; ++i) {
        dst[i] = '\0';
    }

    return dst;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    while (n-- > 0) {
        if (*s1 != *s2) {
            return (unsigned char)*s1 - (unsigned char)*s2;
        }
        if (*s1 == '\0') {
            return 0;
        }
        s1++;
        s2++;
    }
    return 0;
}

char* strchr(const char* str, int c) {
	unsigned char ch = (unsigned char)c;

	do {
		if ((unsigned char)*str == ch)
			return (char*)str;
	} while (*str++);

	return NULL;
}

char* strrchr(const char* str, int c) {
	const char* last = NULL;
	unsigned char ch = (unsigned char)c;

	do {
		if ((unsigned char)*str == ch)
			last = str;
	} while (*str++);

	return (char*)last;
}

int strcasecmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return (unsigned char)*s1 - (unsigned char)*s2;
        }
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}