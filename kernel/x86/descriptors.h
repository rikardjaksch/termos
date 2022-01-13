#pragma once

#include <kernel/klibc/stdint.h>

#define GDT_NULL_SEGMENT            0
#define GDT_KERNEL_CODE_SEGMENT     1
#define GDT_KERNEL_DATA_SEGMENT     2
#define GDT_USER_CODE_SEGMENT       3
#define GDT_USER_DATA_SEGMENT       4
#define GDT_SEGMENT_COUNT           5

#define GDT_NULL_SEGMENT_SELECTOR            0x00
#define GDT_KERNEL_CODE_SEGMENT_SELECTOR     0x08
#define GDT_KERNEL_DATA_SEGMENT_SELECTOR     0x10
#define GDT_USER_CODE_SEGMENT_SELECTOR       0x18
#define GDT_USER_DATA_SEGMENT_SELECTOR       0x20

union descriptor {
    // GDT
    struct {
        uint16_t limit_low;
        uint16_t base_low;
        uint8_t base_mid;
        uint8_t access;
        uint8_t limit_high : 4;
        uint8_t flags : 4;
        uint8_t base_high;
    } segment;

    // IDT
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

descriptor_t make_gdt_entry(uint32_t base, uint32_t limit, uint8_t flags, uint8_t access);
void flush_gdt_table(descriptor_table_pointer_t table);