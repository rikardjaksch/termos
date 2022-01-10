#include <kernel/kprintf.h>

// Internal output function
typedef void (*out_func_type)(
    char character,
    void* buffer,
    size_t index,
    size_t max_length
);

typedef struct {
    void(*func)(char character, void* arg);
    void* arg;
} out_func_wrapper_t;

int kprintf(void(*out)(char character, void* arg), void* user_data, const char* format, ...) {
    va_list va;
    va_start(va, format);
    va_end(va);    
}