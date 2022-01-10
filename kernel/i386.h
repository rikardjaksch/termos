#pragma once

#include <kernel/types.h>

#define cli() asm volatile("cli")
#define sti() asm volatile("sti")

void gdt_init();

void idt_init();
void register_interrupt_handler(uint8_t isr_number, void(*f)());