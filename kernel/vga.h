#pragma once

namespace vga {
    enum class color {
        Black,
        Blue,
        Green,
        Cyan,
        Red,
        Magenta,
        Brown,
        LightGrey,
        DarkGrey,
    };

    void initialize(color bg, color fg);
    void clear_screen();
    void place_cursor(int x, int y);
    void putch(unsigned char c);
    void putch_at(char c, int x, int y);
}
