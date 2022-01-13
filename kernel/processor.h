#pragma once

#include <kernel/klibc/stdint.h>

typedef enum exception_type {
    EX_0,
    EX_1,
    EX_2,
    EX_3,
    EX_4,
    EX_5,
    EX_6,
    EX_7,
    EX_8,
    EX_9,
    EX_10,
    EX_11,
    EX_12,
    EX_13,
    EX_14,
    EX_15,
    EX_16,
} exception_type_t;

typedef enum irq_selector {
    IRQ_PIT,
    IRQ_PS2_KEYBOARD,
    IRQ_CASCADE,
    IRQ_COM2,
    IRQ_COM1,
    IRQ_LPT2,
    IRQ_FLOPPY,
    IRQ_LPT1,
    IRQ_CMOS_RTC,
    IRQ_9,
    IRQ_10,
    IRQ_11,
    IRQ_PS2_MOUSE,
    IRQ_FPU,
    IRQ_PRIMARY_ATA,
    IRQ_SECONDARY_ATA,
} irq_selector_t;

struct interrupt_data {    
    uint16_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t isr_vec, error_code;
    uint32_t eip, cs, eflags, user_esp, ss;
} __attribute__((packed));
typedef struct interrupt_data interrupt_data_t;

typedef void (*interrupt_handler_t)(interrupt_data_t);

struct processor_api {
    void (*initialize)();

    void (*register_exception_handler)(exception_type_t, interrupt_handler_t);
    void (*register_irq_handler)(irq_selector_t, interrupt_handler_t);

    void (*disable_interrupts)();
    void (*enable_interrupts)();
};

extern struct processor_api* processor_api;