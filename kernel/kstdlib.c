#include <kernel/kstdlib.h>

void* memcpy(void* destination, const void* source, size_t n) {
    char* dst = (char*)destination;
    char* src = (char*)source;

    for (size_t i = 0; i < n; ++i)
        dst[i] = src[i];

    return destination;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (*(str++))
        ++len;
    return len;
}