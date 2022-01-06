/*
 * TODO:
 *  - Add to git so I can work from home as well
 *  - 
 *
 */

#include <kernel/vga.h>

extern "C" [[noreturn]] void kernel_main() {
    vga::initialize(vga::color::Black, vga::color::Green);
    vga::putch('H');
    vga::putch('e');
    vga::putch('l');
    vga::putch('l');
    vga::putch('o');
    vga::putch('!');

    for (;;) {
        asm("hlt");
    }
}
