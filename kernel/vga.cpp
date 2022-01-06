#include <kernel/vga.h>

namespace vga {
    static unsigned char* vga_memory = nullptr;

    void initialize() {
        vga_memory = (unsigned char*)0xb8000;
        clear_screen();

        // Set cursor position
    }

    void clear_screen() {
        for (int i = 0; i < 25; ++i) {
            unsigned short* line = (unsigned short*)&vga_memory[i * 160];
            for (int j = 0; j < 80; ++j) {
                line[j] = 0x0720;
            }
        }
    }
}
