/*
 * TODO:
 *  - Debug logging
 *  - GDT and IDT setup
 *  - Multiboot info structures
 */

#include <kernel/vga.h>
#include <kernel/multiboot.h>

multiboot_info_t* multiboot_info_ptr;

void kernel_main() {
    vga_initialize(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
    vga_putstr("Entering Termos\nKernel");


    for (;;) {
        asm("hlt");
    }
}
