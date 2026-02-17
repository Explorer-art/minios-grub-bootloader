#ifndef CONFIG_H
#define CONFIG_H

#include <fs/fat32.h>
#include <stddef.h>
#include <stdbool.h>

bool config_get_str(fat32_file_t* file, const char* key, char* value, size_t size);
bool config_get_int(fat32_file_t* file, const char* key, int* value);

#endif