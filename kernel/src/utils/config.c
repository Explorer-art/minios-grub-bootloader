#include <utils/config.h>
#include <utils/kprintf.h>
#include <string.h>

bool config_get_str(fat32_file_t* file, const char* key, char* out, size_t size) {
    char buffer[512];
    size_t bytes_read;

    while ((bytes_read = fat32_read_line(file, buffer, sizeof(buffer))) > 0) {
        char* current_key = strtok(buffer, "=");
        char* value = strtok(NULL, "=");

        if (current_key && value && strcmp(current_key, key) == 0) {
            strncpy(out, value, size - 1);
            out[size - 1] = '\0';
            return true;
        }
    }

    return false;
}

bool config_get_int(fat32_file_t* file, const char* key, int* value) {}