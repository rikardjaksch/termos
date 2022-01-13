#include <kernel/processor.h>
#include <kernel/debug/log.h>
#include <kernel/x86/descriptors.h>

static descriptor_table_pointer_t gdtr;
//static descriptor_table_pointer_t idtr;

static descriptor_t gdt_descriptors[GDT_SEGMENT_COUNT];
//static descriptor_t idt_descriptors[256];

void gdt_init() {
    gdt_descriptors[GDT_NULL_SEGMENT] = make_gdt_entry(0x0, 0x0, 0x0, 0x0);
    gdt_descriptors[GDT_KERNEL_CODE_SEGMENT] = make_gdt_entry(0x0, 0xffffffff, 0xc, 0x9a); // Kernel code segment
    gdt_descriptors[GDT_KERNEL_DATA_SEGMENT] = make_gdt_entry(0x0, 0xffffffff, 0xc, 0x92); // Kernel data segment
    gdt_descriptors[GDT_USER_CODE_SEGMENT] = make_gdt_entry(0x0, 0xffffffff, 0xc, 0xfa); // User code segment
    gdt_descriptors[GDT_USER_DATA_SEGMENT] = make_gdt_entry(0x0, 0xffffffff, 0xc, 0xf2); // User data segment

    gdtr.offset = gdt_descriptors;
    gdtr.size = (sizeof(descriptor_t) * 5) - 1;
    //flush_gdt();

    flush_gdt_table(gdtr);

    asm volatile(
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        :: "a"(GDT_KERNEL_DATA_SEGMENT_SELECTOR)
        : "memory");

      asm volatile(
        "ljmpl $0x8, $sanity\n"
        "sanity:\n");
}

void processor_initialize() {
    klog_info("Initializing processor\n");

    klog_debug("Setting up the GDT\n");
    gdt_init();

    klog_debug("Setting up the IDT\n");
    // Initialize IDT
}

void processor_clear_interrupts() {
    asm volatile("cli");
}

struct processor_api* processor_api = &(struct processor_api) {    
    .initialize = processor_initialize,
    .clear_interrupts = processor_clear_interrupts
};