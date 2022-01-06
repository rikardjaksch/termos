#include <kernel/vga.h>
#include <kernel/io.h>

namespace vga {
    static constexpr int32_t WIDTH = 80;
    static constexpr int32_t HEIGHT = 25;
    static constexpr int32_t AREA = WIDTH * HEIGHT;

    static uint16_t* vga_memory = nullptr;

    static int cursor_x = 0;
    static int cursor_y = 0;

    static uint8_t terminal_color = 0;

    void clamp_position(int& x, int& y) {
        if (x < 0) x = 0;
        if (x >= WIDTH) x = WIDTH - 1;
        if (y < 0) y = 0;
        if (y >= HEIGHT) y = HEIGHT - 1;
    }

    void increment_cursor() {
        cursor_x++;

        if (cursor_x >= WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }

        if (cursor_y >= HEIGHT) {
            // Scroll screen, for now, just wrap around
            cursor_y = 0;
        }

        place_cursor(cursor_x, cursor_y);
    }

    uint8_t vga_entry_color(color bg, color fg) {
        return (uint8_t)fg | (uint8_t)bg << 4;
    }

    uint16_t vga_entry(unsigned char c, uint8_t color) {
        return (uint16_t)c | (uint16_t)color << 8;
    }

    void initialize(color bg, color fg) {
        vga_memory = (uint16_t*)0xb8000;
        terminal_color = vga_entry_color(bg, fg);
        clear_screen();
        place_cursor(0, 0);
    }

    void clear_screen() {
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                const uint16_t index = y * WIDTH + x;
                vga_memory[index] = vga_entry(' ', terminal_color);
            }
        }

        // Reset cursor after clearing
        place_cursor(0, 0);
    }

    void place_cursor(int x, int y) {
      clamp_position(x, y);
      cursor_x = x;
      cursor_y = y;

      io::out8(0x3d4, 0x0e);
      io::out8(0x3d5, y);
      io::out8(0x3d4, 0x0f);
      io::out8(0x3d5, x);
    }

    void putch(unsigned char c) {
        const uint16_t index = cursor_y * WIDTH + cursor_x;
        vga_memory[index] = vga_entry(c, terminal_color);
        increment_cursor();
    }
}
