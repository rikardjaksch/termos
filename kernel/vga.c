#include <kernel/vga.h>
#include <kernel/io.h>
#include <kernel/printf_impl.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_AREA (VGA_WIDTH * VGA_HEIGHT)

static uint16_t* vga_memory = 0;

static int cursor_x = 0;
static int cursor_y = 0;
static uint8_t terminal_color = 0;

void clamp_position(int* x, int* y) {
    if (*x < 0) x = 0;
    if (*x >= VGA_WIDTH) *x = VGA_WIDTH - 1;
    if (*y < 0) y = 0;
    if (*y >= VGA_HEIGHT) *y = VGA_HEIGHT - 1;
}

void place_cursor(int x, int y) {
    clamp_position(&x, &y);
    cursor_x = x;
    cursor_y = y;

    io_out8(0x3d4, 0x0e);
    io_out8(0x3d5, y);
    io_out8(0x3d4, 0x0f);
    io_out8(0x3d5, x);
}

void increment_cursor() {
    cursor_x++;

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= VGA_HEIGHT) {
        // Scroll screen, for now, just wrap around
        cursor_y = 0;
    }

    place_cursor(cursor_x, cursor_y);
}

void new_line() {
    cursor_x = 0;
    cursor_y++;

    if (cursor_y >= VGA_HEIGHT) {
        // Scroll screen, for now, just wrap around
        cursor_y = 0;
    }

    place_cursor(cursor_x, cursor_y);
}

uint8_t vga_entry_color(vga_color_e bg, vga_color_e fg) {
    return (uint8_t)fg | (uint8_t)bg << 4;
}

uint16_t vga_entry(unsigned char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

void vga_initialize(vga_color_e bg, vga_color_e fg) {
    vga_memory = (uint16_t*)0xb8000;
    terminal_color = vga_entry_color(bg, fg);
    vga_clear_screen();
    place_cursor(0, 0);
}

void vga_clear_screen() {
    for (int y = 0; y < VGA_HEIGHT; ++y) {
        for (int x = 0; x < VGA_WIDTH; ++x) {
            const uint16_t index = y * VGA_WIDTH + x;
            vga_memory[index] = vga_entry(' ', terminal_color);
        }
    }

    // Reset cursor after clearing
    place_cursor(0, 0);
}

void vga_putch(unsigned char c) {
    if (c == '\n') {
        new_line();
        return;
    }

    const uint16_t index = cursor_y * VGA_WIDTH + cursor_x;
    vga_memory[index] = vga_entry(c, terminal_color);
    increment_cursor();
}

void vga_putstr(const char* str) {
    while(*str != '\0') {
        vga_putch(*(str++));
    }
}

void vga_printf(const char* fmt, ...) {

}
