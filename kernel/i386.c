#include <kernel/i386.h>
#include <kernel/vga.h>

union descriptor {
    struct {
        uint16_t limit_low;
        uint16_t base_low;
        uint8_t base_mid;
        uint8_t access;
        uint8_t limit_high : 4;
        uint8_t flags : 4;
        uint8_t base_high;
    } segment;

    struct {
        uint16_t offset_low;
        uint16_t selector;
        uint8_t zero;
        uint8_t type_attributes;
        uint16_t offset_high;
    } gate;
} __attribute__((packed));
typedef union descriptor descriptor_t;

struct descriptor_table_pointer {
    uint16_t size;
    void* offset;
} __attribute__((packed));
typedef struct descriptor_table_pointer descriptor_table_pointer_t;

static descriptor_table_pointer_t gdtr;
static descriptor_table_pointer_t idtr;

static descriptor_t gdt_descriptors[5];
static descriptor_t idt_descriptors[256];

// Generic exception handler
#define EH(i, msg) \
    static void _exception ## i () \
    { \
        vga_printf("Unhandled interrupt\n"); \
        asm volatile("cli; hlt"); \
    }

EH(0, "Divide error")
EH(1, "Debug exception")
EH(2, "Unknown error")
EH(3, "Breakpoint")
EH(4, "Overflow")
EH(5, "Bounds check")
EH(6, "Invalid opcode")
EH(7, "Coprocessor not available")
EH(8, "Double fault")
EH(9, "Coprocessor segment overrun")
EH(10, "Invalid TSS")
EH(11, "Segment not present")
EH(12, "Stack exception")
EH(13, "General protection fault")
EH(14, "Page fault")
EH(15, "Unknown error")
EH(16, "Coprocessor error")

void write_gdt_entry(uint32_t index, uint32_t base, uint32_t limit, uint8_t flags, uint8_t access) {
    descriptor_t* desc = &gdt_descriptors[index];

    desc->segment.flags = flags;
    desc->segment.access = access;
    
    desc->segment.limit_low = (limit & 0xffff);
    desc->segment.limit_high = ((limit >> 16) & 0x0f);
    
    desc->segment.base_low = (base & 0xffff);
    desc->segment.base_mid = ((base >> 16) & 0xff);
    desc->segment.base_high = ((base >> 24) & 0xff);
}

// Used for exceptions that we don't need to handle in any kind of special way
void unimplemented_trap() {
    vga_printf("Unhandled interrupt\n");
    asm volatile("cli; hlt");
}

void flush_gdt() {    
    asm("lgdt %0" :: "m"(gdtr));
}

void flush_idt() {
    asm("lidt %0" :: "m"(idtr));
}

void gdt_init() {
    gdtr.offset = gdt_descriptors;
    gdtr.size = (sizeof(descriptor_t) * 5) - 1;

    write_gdt_entry(0, 0x0, 0x0, 0x0, 0x0);
    write_gdt_entry(1, 0x0, 0xffffffff, 0xc, 0x9a); // Kernel code segment
    write_gdt_entry(2, 0x0, 0xffffffff, 0xc, 0x92); // Kernel data segment
    write_gdt_entry(3, 0x0, 0xffffffff, 0xc, 0xfa); // User code segment
    write_gdt_entry(4, 0x0, 0xffffffff, 0xc, 0xf2); // User data segment

    flush_gdt();

    asm volatile(
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%ss\n" ::"a"(0x10)
        : "memory");
}

void register_interrupt_handler(uint8_t isr_number, void(*f)()) {
    descriptor_t* desc = &idt_descriptors[isr_number];
    
    desc->gate.zero = 0;
    desc->gate.selector = 0x8;
    desc->gate.offset_low = ((uint32_t)f & 0x0000ffff);
    desc->gate.offset_high = ((uint32_t)f & 0xffff0000);
    desc->gate.type_attributes = 0x8e;

    flush_idt();
}

void irq7_handler();
asm(
    ".globl irq7_handler \n"
    "irq7_handler: \n"
    "   iret\n"
);

void idt_init() {
    idtr.offset = idt_descriptors;
    idtr.size = (sizeof(descriptor_t) * 256);

    // Set a default interrupt handler for all interrupt vectors.
    // Skip the first 16 since those are important exceptions that
    // we might care about.
    for (uint8_t i = 0xff; i > 0x10; --i) {
        register_interrupt_handler(i, unimplemented_trap);
    }

    // Register all exception handlers
    register_interrupt_handler(0x00, _exception0);
    register_interrupt_handler(0x01, _exception1);
    register_interrupt_handler(0x02, _exception2);
    register_interrupt_handler(0x03, _exception3);
    register_interrupt_handler(0x04, _exception4);
    register_interrupt_handler(0x05, _exception5);
    register_interrupt_handler(0x06, _exception6);
    register_interrupt_handler(0x07, _exception7);
    register_interrupt_handler(0x08, _exception8);
    register_interrupt_handler(0x09, _exception9);
    register_interrupt_handler(0x0a, _exception10);
    register_interrupt_handler(0x0b, _exception11);
    register_interrupt_handler(0x0c, _exception12);
    register_interrupt_handler(0x0d, _exception13);
    register_interrupt_handler(0x0e, _exception14);
    register_interrupt_handler(0x0f, _exception15);
    register_interrupt_handler(0x10, _exception16);

    register_interrupt_handler(0x57, irq7_handler);

    flush_idt();
}