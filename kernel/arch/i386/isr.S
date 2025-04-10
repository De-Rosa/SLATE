# https://wiki.osdev.org/Interrupts_Tutorial
# http://www.osdever.net/bkerndev/Docs/isrs.htm

.macro isr_err_stub n
.global isr_stub_\n
isr_stub_\n:
    pushl $\n
    call isr_common_stub
    iret
.endm

.macro isr_no_err_stub n
.global isr_stub_\n
isr_stub_\n:
    pushl $0
    pushl $\n
    call isr_common_stub
    iret
.endm

.extern exception_handler

.global isr_common_stub
isr_common_stub:
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    movl %esp, %eax
    pushl %eax

    call exception_handler

    popl %eax
    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

    sti
    
    addl $8, %esp
    iret

isr_no_err_stub 0   // Division by zero exception
isr_no_err_stub 1   // Debug exception
isr_no_err_stub 2   // Non Maskable Interrupt exception
isr_no_err_stub 3   // Breakpoint exception
isr_no_err_stub 4   // Info detected overflow exception
isr_no_err_stub 5   // Out of bounds exception
isr_no_err_stub 6   // Invalid opcode exception
isr_no_err_stub 7   // No coprocessor exception
isr_err_stub    8   // Double fault exception
isr_no_err_stub 9   // Coprocessor segment overrun exception
isr_err_stub   10   // Bad TSS exception
isr_err_stub   11   // Segment not present exception
isr_err_stub   12   // Stack fault exception
isr_err_stub   13   // General protection fault exception
isr_err_stub   14   // Page fault exception
isr_no_err_stub 15  // Unknown interrupt exception
isr_no_err_stub 16  // Coprocessor fault exception
isr_err_stub   17   // Alignment check exception
isr_no_err_stub 18  // Machine check exception
isr_no_err_stub 19  // Reserved
isr_no_err_stub 20  // Reserved
isr_no_err_stub 21  // Reserved
isr_no_err_stub 22  // Reserved
isr_no_err_stub 23  // Reserved
isr_no_err_stub 24  // Reserved
isr_no_err_stub 25  // Reserved
isr_no_err_stub 26  // Reserved
isr_no_err_stub 27  // Reserved
isr_no_err_stub 28  // Reserved
isr_no_err_stub 29  // Reserved
isr_err_stub   30   // Reserved
isr_no_err_stub 31  // Reserved

.global isr_table
isr_table:
    .long isr_stub_0
    .long isr_stub_1
    .long isr_stub_2
    .long isr_stub_3
    .long isr_stub_4
    .long isr_stub_5
    .long isr_stub_6
    .long isr_stub_7
    .long isr_stub_8
    .long isr_stub_9
    .long isr_stub_10
    .long isr_stub_11
    .long isr_stub_12
    .long isr_stub_13
    .long isr_stub_14
    .long isr_stub_15
    .long isr_stub_16
    .long isr_stub_17
    .long isr_stub_18
    .long isr_stub_19
    .long isr_stub_20
    .long isr_stub_21
    .long isr_stub_22
    .long isr_stub_23
    .long isr_stub_24
    .long isr_stub_25
    .long isr_stub_26
    .long isr_stub_27
    .long isr_stub_28
    .long isr_stub_29
    .long isr_stub_30
    .long isr_stub_31
