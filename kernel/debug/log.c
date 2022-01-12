#include <kernel/debug/log.h>
#include <kernel/io.h>
#include <kernel/kprintf.h>

void e9_putch(char c) {
    io_out8(0xe9, c);
}

static const char* level_colors[] = {
    "34",
    "37",
    "32",
    "31",
    "95",
};

void color_on(debug_log_type_e level) {
    io_out8(0xe9, 0x1b);
    io_out8(0xe9, '[');

    const char* level_color = level_colors[level];
    io_out8(0xe9, level_color[0]);
    io_out8(0xe9, level_color[1]);

    //io_out8(0xe9, '3');
    //io_out8(0xe9, '1');
    io_out8(0xe9, 'm');
}

void color_off() {
    io_out8(0xe9, 0x1b);
    io_out8(0xe9, '[');
    io_out8(0xe9, '0');
    io_out8(0xe9, 'm');
}

void klog_log(debug_log_type_e level, const char* fmt, ...) {
    color_on(level);

    va_list ap;
    va_start(ap, fmt);
    printf_implementation(e9_putch, fmt, ap);
    va_end(ap);

    color_off();
}