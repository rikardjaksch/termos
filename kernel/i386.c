#include <kernel/i386.h>


// Most likely the PIT is causing the interrupts that we're getting
// So we should setup the PIT before enabling interrupts

// https://github.com/ozkl/soso/blob/2839f6f9aaa633553a26818dd5e02c0f3ddea545/kernel/interrupt.asm
/*
extern void handle_irq();
extern void asm_irq_entry();

asm(
    ".globl asm_irq_entry\n"
    "asm_irq_entry: \n"
    "    pusha\n"
    "    pushw %ds\n"
    "    pushw %es\n"
    "    pushw %ss\n"
    "    pushw %ss\n"
    "    popw %ds\n"
    "    popw %es\n"
    "    call handle_irq\n"
    "    popw %es\n"
    "    popw %ds\n"
    "    popa\n"
    "    iret\n"
);
*/
/*#define EH_ENTRY(ec) \
extern void exception_ ## ec ## _handler(register_dump_t*); \
extern void exception_ ## ec ## _entry(); \
asm( \
    ".globl exception_" #ec "_entry\n" \
    "exception_" #ec "_entry: \n" \
    "    pushw $" #ec "\n" \
    "    pusha\n" \
    "    pushw %ds\n" \
    "    pushw %es\n" \
    "    pushw %fs\n" \
    "    pushw %gs\n" \
    "    pushl %ebx\n" \
    "    mov $0x10, %bx\n" \
    "    mov %bx, %ds\n" \
    "    mov %bx, %es\n" \
    "    mov %bx, %fs\n" \
    "    mov %bx, %gs\n" \
    "    popl %ebx\n" \
    "    call exception_" # ec "_handler\n" \
    "    popw %gs\n" \
    "    popw %fs\n" \
    "    popw %es\n" \
    "    popw %ds\n" \
    "    popa\n" \
    "    add $0x4, %esp\n" \
    "    iret\n" \
);

#define EH_ENTRY_NO_CODE(ec) \
extern void exception_ ## ec ## _handler(register_dump_t*); \
extern void exception_ ## ec ## _entry(); \
asm( \
    ".globl exception_" #ec "_entry\n" \
    "exception_" #ec "_entry: \n" \
    "    pushw $0x0\n" \
    "    pushw $" #ec "\n" \
    "    pusha\n" \
    "    pushw %ds\n" \
    "    pushw %es\n" \
    "    pushw %fs\n" \
    "    pushw %gs\n" \
    "    pushl %ebx\n" \
    "    mov $0x10, %bx\n" \
    "    mov %bx, %ds\n" \
    "    mov %bx, %es\n" \
    "    mov %bx, %fs\n" \
    "    mov %bx, %gs\n" \
    "    popl %ebx\n" \
    "    call exception_" # ec "_handler\n" \
    "    popw %gs\n" \
    "    popw %fs\n" \
    "    popw %es\n" \
    "    popw %ds\n" \
    "    popa\n" \
    "    add $0x8, %esp\n" \
    "    iret\n" \
);


EH_ENTRY_NO_CODE(0)
void exception_0_handler(register_dump_t*) {
    asm volatile("hlt");
}

EH_ENTRY_NO_CODE(1)
void exception_1_handler(register_dump_t*) {
    vga_printf("");
    asm volatile("hlt");
}

EH_ENTRY_NO_CODE(2)
void exception_2_handler(register_dump_t*) {
    asm volatile("hlt");
}

EH_ENTRY_NO_CODE(3)
void exception_3_handler(register_dump_t*) {
    asm volatile("hlt");
}

EH_ENTRY_NO_CODE(4)
void exception_4_handler(register_dump_t*) {
    asm volatile("hlt");
}

EH_ENTRY_NO_CODE(5)
void exception_5_handler(register_dump_t*) {
    asm volatile("hlt");
}
EH_ENTRY_NO_CODE(6)
void exception_6_handler(register_dump_t*) {
    asm volatile("hlt");
}
EH_ENTRY_NO_CODE(7)
void exception_7_handler(register_dump_t*) {
    asm volatile("hlt");
}
EH_ENTRY(8)
void exception_8_handler(register_dump_t*) {
    asm volatile("hlt");
}
EH_ENTRY_NO_CODE(9)
void exception_9_handler(register_dump_t*) {
    asm volatile("hlt");
}
EH_ENTRY(10)
void exception_10_handler(register_dump_t*) {
    asm volatile("hlt");
}
EH_ENTRY(11)
void exception_11_handler(register_dump_t*) {
    asm volatile("hlt");
}
EH_ENTRY(12)
void exception_12_handler(register_dump_t*) {
    asm volatile("hlt");
}
EH_ENTRY(13)
void exception_13_handler(register_dump_t*) {
    asm volatile("hlt");
}
EH_ENTRY(14)
void exception_14_handler(register_dump_t*) {
    asm volatile("hlt");
}
EH_ENTRY_NO_CODE(15)
void exception_15_handler(register_dump_t*) {
    asm volatile("hlt");
}
EH_ENTRY_NO_CODE(16)
void exception_16_handler(register_dump_t*) {
    asm volatile("hlt");
}
*/
/*void handle_irq() {
    // Fetch ISR number
    io_out8(PIC1, 0x0b);
    io_out8(PIC2, 0x0b);
    uint8_t isr0 = io_in8(PIC1);
    uint8_t isr1 = io_in8(PIC2);
    uint16_t isr = (isr1 << 8) | isr0;
    
    if (!isr) {
        vga_printf("Spurious IRQ\n");
        return;
    }

    uint8_t irq = 0;
    for (uint8_t i = 0; i < 16; ++i) {
        if (i == 2)
            continue;
        if (isr & (1 << i)) {
            irq = i;
            break;
        }
    }

    //if (s_irq_handler[irq])
    //    s_irq_handler[irq]->handle_irq();

    if (irq & 8)
        io_out8(PIC2, 0x20);
    io_out8(PIC1, 0x20);
    //PIC::eoi(irq);
}*/