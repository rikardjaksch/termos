#pragma once

#include <kernel/klibc/stdint.h>

#define cli() asm volatile("cli")
#define sti() asm volatile("sti")

struct register_dump {    
    uint16_t gs;
    uint16_t fs;
    uint16_t es;
    uint16_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t isr_vec, error_code;
    uint32_t eip, cs, eflags, user_esp, ss;
} __attribute__((packed));
typedef struct register_dump register_dump_t;

void gdt_init();

void idt_init();
void register_interrupt_handler(uint8_t isr_number, uint32_t base);