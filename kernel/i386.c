#include <kernel/i386.h>
#include <kernel/vga.h>
#include <kernel/klibc/stdlib.h>
#include <kernel/io.h>

#define PIC1		    0x20
#define PIC1_OFFSET     0x20
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	    (PIC1+1)

#define PIC2		    0xA0
#define PIC2_OFFSET     0x28
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	    (PIC2+1)

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

// Most likely the PIT is causing the interrupts that we're getting
// So we should setup the PIT before enabling interrupts

// https://github.com/ozkl/soso/blob/2839f6f9aaa633553a26818dd5e02c0f3ddea545/kernel/interrupt.asm

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

#define EH_ENTRY(ec) \
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

static descriptor_table_pointer_t gdtr;
static descriptor_table_pointer_t idtr;

static descriptor_t gdt_descriptors[5];
static descriptor_t idt_descriptors[256];

void write_gdt_entry(uint32_t index, uint32_t base, uint32_t limit, uint8_t flags, uint8_t access) {
    descriptor_t* desc = &gdt_descriptors[index];

    desc->segment.flags = flags;
    desc->segment.access = access;
    
    desc->segment.limit_low = (limit & 0xffff);
    desc->segment.limit_high = (limit >> 16) & 0x0f;
    
    desc->segment.base_low = (base & 0xffff);
    desc->segment.base_mid = (base >> 16) & 0xff;
    desc->segment.base_high = (base >> 24) & 0xff;
}

void flush_gdt() {    
    asm("lgdt %0" :: "m"(gdtr):"memory");
}

void flush_idt() {
    asm("lidt %0" :: "m"(idtr));
}

void remap_pic() {
    unsigned char a1, a2;
 
    /* Save mask */
	a1 = io_in8(PIC1_DATA);
	a2 = io_in8(PIC2_DATA);

    /* Cascade initialization */
    io_out8(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); io_wait();
	io_out8(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4); io_wait();

    /* Remap */
	io_out8(PIC1_DATA, PIC1_OFFSET); io_wait();
	io_out8(PIC2_DATA, PIC2_OFFSET); io_wait();

    /* Cascade identity with slave PIC at IRQ2 */
	io_out8(PIC1_DATA, 0x04); io_wait();
	io_out8(PIC2_DATA, 0x02); io_wait();

    /* Request 8086 mode on each PIC */
	io_out8(PIC1_DATA, ICW4_8086); io_wait();
	io_out8(PIC2_DATA, ICW4_8086); io_wait();

    /* Restore saved mask */
    io_out8(PIC1_DATA, a1);
	io_out8(PIC2_DATA, a2);
}

void gdt_init() {
    write_gdt_entry(0, 0x0, 0x0, 0x0, 0x0);
    write_gdt_entry(1, 0x0, 0xffffffff, 0xc, 0x9a); // Kernel code segment
    write_gdt_entry(2, 0x0, 0xffffffff, 0xc, 0x92); // Kernel data segment
    write_gdt_entry(3, 0x0, 0xffffffff, 0xc, 0xfa); // User code segment
    write_gdt_entry(4, 0x0, 0xffffffff, 0xc, 0xf2); // User data segment

    gdtr.offset = gdt_descriptors;
    gdtr.size = (sizeof(descriptor_t) * 5) - 1;
    flush_gdt();

    asm volatile(
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        :: "a"(0x10)
        : "memory");

      asm volatile(
        "ljmpl $0x8, $sanity\n"
        "sanity:\n");
}

void register_interrupt_handler(uint8_t isr_number, uint32_t base) {
    descriptor_t* desc = &idt_descriptors[isr_number];
    
    desc->gate.zero = 0;
    desc->gate.selector = 0x8;
    desc->gate.offset_low = base & 0xffff;
    desc->gate.offset_high = (base >> 16) & 0xffff;
    desc->gate.type_attributes = 0x8e;

    flush_idt();
}

static void unimp_trap(){
    vga_printf("Unhandled IRQ.");
    asm volatile("hlt");
}

void idt_init() {
    idtr.offset = idt_descriptors;
    idtr.size = (sizeof(descriptor_t) * 256);
    memset(idt_descriptors, 0, sizeof(descriptor_t) * 256);

    for (uint8_t i = 0xff; i > 0x10; --i)
        register_interrupt_handler(i, (uint32_t)unimp_trap);

    register_interrupt_handler(0, (uint32_t)exception_0_entry);
    register_interrupt_handler(0, (uint32_t)exception_1_entry);
    register_interrupt_handler(0, (uint32_t)exception_2_entry);
    register_interrupt_handler(0, (uint32_t)exception_3_entry);
    register_interrupt_handler(0, (uint32_t)exception_4_entry);
    register_interrupt_handler(0, (uint32_t)exception_5_entry);
    register_interrupt_handler(0, (uint32_t)exception_6_entry);
    register_interrupt_handler(0, (uint32_t)exception_7_entry);
    register_interrupt_handler(0, (uint32_t)exception_8_entry);
    register_interrupt_handler(0, (uint32_t)exception_9_entry);
    register_interrupt_handler(0, (uint32_t)exception_10_entry);
    register_interrupt_handler(0, (uint32_t)exception_11_entry);
    register_interrupt_handler(0, (uint32_t)exception_12_entry);
    register_interrupt_handler(0, (uint32_t)exception_13_entry);
    register_interrupt_handler(0, (uint32_t)exception_14_entry);
    register_interrupt_handler(0, (uint32_t)exception_15_entry);
    register_interrupt_handler(0, (uint32_t)exception_16_entry);

    remap_pic();

    flush_idt();
}

void handle_irq() {
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
}