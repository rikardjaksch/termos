//.global isr_0
//.type isr_0, @function
//.isr_0:
//    cli;
//    pushw $0;
//    pushw $0;
//    jmp isr_common_stub;


//EH_ENTRY_NO_CODE(0)
//EH_ENTRY_NO_CODE(1)
//EH_ENTRY_NO_CODE(2)
//EH_ENTRY_NO_CODE(3)
//EH_ENTRY_NO_CODE(4)
//EH_ENTRY_NO_CODE(5)
//EH_ENTRY_NO_CODE(6)
//EH_ENTRY_NO_CODE(7)
//EH_ENTRY(8)
//EH_ENTRY_NO_CODE(9)
//EH_ENTRY(10)
//EH_ENTRY(11)
//EH_ENTRY(12)
//EH_ENTRY(13)
//EH_ENTRY(14)
//EH_ENTRY_NO_CODE(15)
//EH_ENTRY_NO_CODE(16)
//EH_ENTRY_NO_CODE(17)
//EH_ENTRY_NO_CODE(18)
//EH_ENTRY_NO_CODE(19)
//EH_ENTRY_NO_CODE(20)
//EH_ENTRY_NO_CODE(21)
//EH_ENTRY_NO_CODE(22)
//EH_ENTRY_NO_CODE(23)
//EH_ENTRY_NO_CODE(24)
//EH_ENTRY_NO_CODE(25)
//EH_ENTRY_NO_CODE(26)
//EH_ENTRY_NO_CODE(27)
//EH_ENTRY_NO_CODE(28)
//EH_ENTRY_NO_CODE(29)
//EH_ENTRY_NO_CODE(30)
//EH_ENTRY_NO_CODE(31)

#define SAVE_REGS   \
    pusha;          \
    pushw  %ds;     \
    pushw  %es;     \
    pushw  %fs;     \
    pushw  %gs;     \

    pushl %ebx;     \
    mov $0x10, %bx; \
    mov %bx, %ds;   \
    mov %bx, %es;   \
    mov %bx, %fs;   \
    mov %bx, %gs;   \
    popl %ebx;

#define RESTORE_REGS \
    popw %gs;        \
    popw %fs;        \
    popw %es;        \
    popw %ds;        \
    popa;

//isr_common_stub:
//    SAVE_REGS
//    /* Call C handler */
//    RESTORE_REGS
//    add $0x8, %esp
//    iret