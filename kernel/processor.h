#pragma once

/*
typedef enum execption_type {

} execption_type_t;

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

typedef struct exception_data {
    uint32 eax;
    // ....
} exception_data_t;

typedef struct irq_data {
    uint32 eax;
    // ....
} irq_data_t;

typedef void (*exception_handler_t)(exception_data_t*);
typedef void (*irq_handler_t)(irq_data_t*);
*/

struct processor_api {
    void (*initialize)();

    /*
        void (register_exception_handler)(execption_type_t, exception_handler_t);
        void (register_irq_handler)(irq_selector_t, irq_handler_t);
    */

    void (*clear_interrupts)();
};

extern struct processor_api* processor_api;