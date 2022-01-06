.set MULTIBOOT_MAGIC,       0x1badb002
.set MULTIBOOT_PAGE_ALIGN,  0x1
.set MULTIBOOT_MEMORY_INFO, 0x2
.set MULTIBOOT_FLAGS,       MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
.set MULTIBOOT_CHECKSUM,    -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

.section .multiboot
.align 4
.long MULTIBOOT_MAGIC
.long MULTIBOOT_FLAGS
.long MULTIBOOT_CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text

.global start
.type start, @function

.extern kernel_main
.type kernel_main, @function

start:
    mov $stack_top, %esp

    cli
    call kernel_main

    cli
1:  hlt
    jmp 1b
