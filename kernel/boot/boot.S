#include <kernel/multiboot.h>

#define MULTIBOOT_FLAGS       MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
#define MULTIBOOT_CHECKSUM    -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_FLAGS)

.section .multiboot
.align 4
.long MULTIBOOT_HEADER_MAGIC
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

.extern multiboot_info_ptr
.type multiboot_info_ptr, @object

start:
    cli
    cld

    mov $stack_top, %esp

    movl %ebx, multiboot_info_ptr

    cli
    call kernel_main

    cli
1:  hlt
    jmp 1b
