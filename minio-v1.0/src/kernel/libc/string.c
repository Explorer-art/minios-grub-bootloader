#include <string.h>
#include <stdint.h>
#include <stdio.h>

char* strcpy(char* dst, char* src) {
  char* orig_dst = dst;

  if (dst == NULL) {
    return NULL;
  }

  if (src == NULL) {
    return NULL;
  }

  while (*src) {
    *dst = *src;
    src++;
    dst++;
  }

  *dst = '\0';

  return orig_dst;
}

const char* strchr(const char* str, char symbol) {
  if (str == NULL) {
    return NULL;
  }

  while (*str) {
    if (*str == symbol) {
      return str;
    }

    str++;
  }

  return NULL;
}

int strcmp(const char* str1, const char* str2) {
  while (true) {
    if (*str1 != *str2) {
      return *str1 > *str2 ? 1 : -1;
    }

    if (*str1 == '\0' && *str1 == '\0') {
      return 0;
    }

    str1++;
    str2++;
  }
}

unsigned strlen(const char* str) {
  unsigned length = 0;

  while (*str) {
    length++;
    str++;
  }

  return length;
}
