#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

size_t strlen(const char*);
char* strcpy(char* dst, const char* src);
char* strncpy(char* dst, const char* src, size_t n);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
int strcasecmp(const char *s1, const char *s2);
char* strchr(const char* str, int c);
char* strrchr(const char* str, int c);
char* strtok(char* str, const char* delim);

#endif