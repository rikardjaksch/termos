#pragma once

typedef enum vga_color {
    VGA_COLOR_BLACK,
    VGA_COLOR_BLUE,
    VGA_COLOR_GREEN,
    VGA_COLOR_CYAN,
    VGA_COLOR_RED,
    VGA_COLOR_MAGENTA,
    VGA_COLOR_BROWN,
    VGA_COLOR_LIGHT_GRAY,
    VGA_COLOR_DARK_GRAY,
} vga_color_e;

void vga_initialize(vga_color_e bg, vga_color_e fg);
void vga_clear_screen();
void vga_place_cursor(int x, int y);
void vga_putch(char c);
void vga_putstr(const char* str);
void vga_printf(const char* fmt, ...);