/*
 * TODO:
 *  - Add to git so I can work from home as well
 *  - 
 *
 */

#include <kernel/vga.h>

extern "C" [[noreturn]] void kernel_main() {
    vga::initialize();
    vga::clear_screen();

    for (;;) {
        asm("hlt");
    }
}
