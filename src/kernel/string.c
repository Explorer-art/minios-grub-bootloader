#include <string.h>
#include <stdint.h>

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

unsigned strlen(const char* str) {
  unsigned length = 0;

  while (*str) {
    length++;
  }

  return length;
}
