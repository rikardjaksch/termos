/*
 * TODO:
 *  - Debug logging
 *  - GDT and IDT setup
 *  - Multiboot info structures
 *  - Need to setup PIT to collect interrupt
 */

#include <kernel/vga.h>
#include <kernel/multiboot.h>
#include <kernel/i386.h>
#include <kernel/debug/log.h>

multiboot_info_t* multiboot_info_ptr = 0;

void kernel_main() {
    klog_debug("Debug message\n");
    klog_info("Info message\n");
    klog_warning("Warning message\n");
    klog_error("Error message\n");
    klog_fatal("Fatal message\n");

    cli();
    
    gdt_init();
    idt_init();

    // Setup vga early so we can print debug stuff to screen
    vga_initialize(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
       
    if (CHECK_MULTI_BOOT_FLAG(multiboot_info_ptr->flags, 6)) {
        multiboot_memory_map_t *mmap = (multiboot_memory_map_t*)multiboot_info_ptr->mmap_addr;

        vga_printf("mmap_addr = %#x, mmap_length = %#x\n",
              (uint32_t)multiboot_info_ptr->mmap_addr, (uint32_t)multiboot_info_ptr->mmap_length);

        for (; (unsigned long) mmap < multiboot_info_ptr->mmap_addr + multiboot_info_ptr->mmap_length;
           mmap = (multiboot_memory_map_t *) ((unsigned long) mmap
                                    + mmap->size + sizeof (mmap->size))) 
        {
            vga_printf(" base_addr = 0x%x%08x,"
                " length = 0x%x%08x, type = 0x%x\n",
                (uint32_t) (mmap->addr >> 32),
                (uint32_t) (mmap->addr & 0xffffffff),
                (uint32_t) (mmap->len >> 32),
                (uint32_t) (mmap->len & 0xffffffff),
                (uint32_t) mmap->type);
        }
    }

    sti();
    
    for (;;) {
    }
}
