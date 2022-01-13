#include <kernel/processor.h>
#include <kernel/debug/log.h>
#include <kernel/io.h>
#include <kernel/x86/descriptors.h>
#include <kernel/klibc/stdlib.h>

#define PIC1		    0x20
#define PIC1_OFFSET     0x20
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	    (PIC1+1)

#define PIC2		    0xA0
#define PIC2_OFFSET     0x28
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	    (PIC2+1)

#define ICW1_ICW4	0x01
#define ICW1_INIT	0x10
#define ICW4_8086	0x01

extern void handle_interrupt(interrupt_data_t);
void unimplemented_trap();

asm( 
    ".globl common_interrupt_handler\n"
    "common_interrupt_handler: \n"
    "    pusha\n"
    "    pushw %ds\n"
    "    pushw %es\n"
    "    pushw %fs\n"
    "    pushw %gs\n"
    "    pushl %ebx\n"
    "    mov $0x10, %bx\n"
    "    mov %bx, %ds\n"
    "    mov %bx, %es\n"
    "    mov %bx, %fs\n"
    "    mov %bx, %gs\n"
    "    popl %ebx\n"

    "    call handle_interrupt\n"

    "    popw %gs\n"
    "    popw %fs\n"
    "    popw %es\n"
    "    popw %ds\n"
    "    popa\n"
    "    add $0x8, %esp\n"
    "    iret\n"
);

#define IRQ_ENTRY(index, byte) \
extern void irq_ ## index ## _entry(); \
asm( \
    ".globl irq_" #index "_entry\n" \
    "irq_" #index "_entry: \n" \
    "    pushw $0x00\n" \
    "    pushw $" #byte "\n" \
    "    jmp common_interrupt_handler\n" \
);

#define EH_ENTRY(ec) \
extern void exception_ ## ec ## _entry(); \
asm( \
    ".globl exception_" #ec "_entry\n" \
    "exception_" #ec "_entry: \n" \
    "    pushw $" #ec "\n" \
    "    jmp common_interrupt_handler\n" \
);

#define EH_ENTRY_NO_ERR_CODE(ec) \
extern void exception_ ## ec ## _entry(); \
asm( \
    ".globl exception_" #ec "_entry\n" \
    "exception_" #ec "_entry: \n" \
    "    pushw $0x00\n" \
    "    pushw $" #ec "\n" \
    "    jmp common_interrupt_handler\n" \
);

EH_ENTRY_NO_ERR_CODE(0)
EH_ENTRY_NO_ERR_CODE(1)
EH_ENTRY_NO_ERR_CODE(2)
EH_ENTRY_NO_ERR_CODE(3)
EH_ENTRY_NO_ERR_CODE(4)
EH_ENTRY_NO_ERR_CODE(5)
EH_ENTRY_NO_ERR_CODE(6)
EH_ENTRY_NO_ERR_CODE(7)
EH_ENTRY(8)
EH_ENTRY_NO_ERR_CODE(9)
EH_ENTRY(10)
EH_ENTRY(11)
EH_ENTRY(12)
EH_ENTRY(13)
EH_ENTRY(14)
EH_ENTRY_NO_ERR_CODE(15)
EH_ENTRY_NO_ERR_CODE(16)

IRQ_ENTRY(0, 32)
IRQ_ENTRY(1, 33)
IRQ_ENTRY(2, 34)
IRQ_ENTRY(3, 35)
IRQ_ENTRY(4, 36)
IRQ_ENTRY(5, 37)
IRQ_ENTRY(6, 38)
IRQ_ENTRY(7, 39)
IRQ_ENTRY(8, 40)
IRQ_ENTRY(9, 41)
IRQ_ENTRY(10, 42)
IRQ_ENTRY(11, 43)
IRQ_ENTRY(12, 44)
IRQ_ENTRY(13, 45)
IRQ_ENTRY(14, 46)
IRQ_ENTRY(15, 47)

static descriptor_table_pointer_t gdtr;
static descriptor_table_pointer_t idtr;

static descriptor_t gdt_descriptors[GDT_SEGMENT_COUNT];
static descriptor_t idt_descriptors[256];

void gdt_init() {
    gdt_descriptors[GDT_NULL_SEGMENT] = make_gdt_entry(0x0, 0x0, 0x0, 0x0);
    gdt_descriptors[GDT_KERNEL_CODE_SEGMENT] = make_gdt_entry(0x0, 0xffffffff, 0xc, 0x9a); // Kernel code segment
    gdt_descriptors[GDT_KERNEL_DATA_SEGMENT] = make_gdt_entry(0x0, 0xffffffff, 0xc, 0x92); // Kernel data segment
    gdt_descriptors[GDT_USER_CODE_SEGMENT] = make_gdt_entry(0x0, 0xffffffff, 0xc, 0xfa); // User code segment
    gdt_descriptors[GDT_USER_DATA_SEGMENT] = make_gdt_entry(0x0, 0xffffffff, 0xc, 0xf2); // User data segment

    gdtr.offset = gdt_descriptors;
    gdtr.size = (sizeof(descriptor_t) * 5) - 1;
    
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

void idt_init() {
    idtr.offset = idt_descriptors;
    idtr.size = (sizeof(descriptor_t) * 256);
    memset(idt_descriptors, 0, sizeof(descriptor_t) * 256);

    // Remap IRQ vectors since they are colliding with the exception vectors
    uint8_t a1 = io_in8(PIC1_DATA);
	uint8_t a2 = io_in8(PIC2_DATA);
    io_out8(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); io_wait();
	io_out8(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4); io_wait();
    io_out8(PIC1_DATA, PIC1_OFFSET); io_wait();
	io_out8(PIC2_DATA, PIC2_OFFSET); io_wait();
    io_out8(PIC1_DATA, 0x04); io_wait();
	io_out8(PIC2_DATA, 0x02); io_wait();
    io_out8(PIC1_DATA, ICW4_8086); io_wait();
	io_out8(PIC2_DATA, ICW4_8086); io_wait();
    io_out8(PIC1_DATA, a1);
	io_out8(PIC2_DATA, a2);

    // Map all exceptions that we might care about
    idt_descriptors[0x0] = make_idt_entry((uint32_t)exception_0_entry);
    idt_descriptors[0x1] = make_idt_entry((uint32_t)exception_1_entry);
    idt_descriptors[0x2] = make_idt_entry((uint32_t)exception_2_entry);
    idt_descriptors[0x3] = make_idt_entry((uint32_t)exception_3_entry);
    idt_descriptors[0x4] = make_idt_entry((uint32_t)exception_4_entry);
    idt_descriptors[0x5] = make_idt_entry((uint32_t)exception_5_entry);
    idt_descriptors[0x6] = make_idt_entry((uint32_t)exception_6_entry);
    idt_descriptors[0x7] = make_idt_entry((uint32_t)exception_7_entry);
    idt_descriptors[0x8] = make_idt_entry((uint32_t)exception_8_entry);
    idt_descriptors[0x9] = make_idt_entry((uint32_t)exception_9_entry);
    idt_descriptors[0xa] = make_idt_entry((uint32_t)exception_10_entry);
    idt_descriptors[0xb] = make_idt_entry((uint32_t)exception_11_entry);
    idt_descriptors[0xc] = make_idt_entry((uint32_t)exception_12_entry);
    idt_descriptors[0xd] = make_idt_entry((uint32_t)exception_13_entry);
    idt_descriptors[0xe] = make_idt_entry((uint32_t)exception_14_entry);
    idt_descriptors[0xf] = make_idt_entry((uint32_t)exception_15_entry);
    idt_descriptors[0x10] = make_idt_entry((uint32_t)exception_16_entry);

    // Set all other to point to unimplemented_trap for now
    for (uint8_t i = 0xff; i > 0x10; --i)
        idt_descriptors[i] = make_idt_entry((uint32_t)unimplemented_trap);

    //register_interrupt_handler(0x20, (uint32_t)pit_handler);

    flush_idt_table(idtr);
}

void unimplemented_trap() {
    klog_error("Unhandled IRQ\n");
    io_out8(PIC1_COMMAND, 0x20);
    //asm volatile("cli; hlt");
}

void handle_interrupt(interrupt_data_t data) {
    klog_error("Unhandled interrupt %d\n", data.isr_vec);
    asm volatile("cli; hlt");
}

void processor_initialize() {
    klog_info("Initializing processor\n");

    klog_debug("Setting up the GDT\n");
    gdt_init();

    klog_debug("Setting up the IDT\n");
    idt_init();
}

void processor_disable_interrupts() {
    asm volatile("cli");
}

void processor_enable_interrupts() {
    asm volatile("sti");
}

void processor_register_exception_handler(exception_type_t, interrupt_handler_t) {
}

struct processor_api* processor_api = &(struct processor_api) {    
    .initialize = processor_initialize,
    .disable_interrupts = processor_disable_interrupts,
    .enable_interrupts = processor_enable_interrupts,
    .register_exception_handler = processor_register_exception_handler,
    .register_irq_handler = 0
};