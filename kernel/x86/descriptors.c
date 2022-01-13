#include <kernel/x86/descriptors.h>

descriptor_t make_gdt_entry(uint32_t base, uint32_t limit, uint8_t flags, uint8_t access) {
    return (descriptor_t){
        .segment.flags = flags,
        .segment.access = access,

        .segment.limit_low = (limit & 0xffff),
        .segment.limit_high = (limit >> 16) & 0x0f,

        .segment.base_low = (base & 0xffff),
        .segment.base_mid = (base >> 16) & 0xff,
        .segment.base_high = (base >> 24) & 0xff
    };
}

void flush_gdt_table(descriptor_table_pointer_t table) {
    asm("lgdt %0" :: "m"(table):"memory");
}

descriptor_t make_idt_entry(uint32_t isr_entry) {
    return (descriptor_t){
        .gate.zero = 0,
        .gate.selector = GDT_KERNEL_CODE_SEGMENT_SELECTOR,
        .gate.offset_low = isr_entry & 0xffff,
        .gate.offset_high = (isr_entry >> 16) & 0xffff,
        .gate.type_attributes = 0x8e
    };
}

void flush_idt_table(descriptor_table_pointer_t table) {
    asm("lidt %0" :: "m"(table));
}