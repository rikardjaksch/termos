#pragma once

#define NULL 0

typedef __SIZE_TYPE__ size_t;

void* memcpy(void* destination, const void* source, size_t n);
void* memset(void* destination, int c, size_t n);
size_t strlen(const char* str);