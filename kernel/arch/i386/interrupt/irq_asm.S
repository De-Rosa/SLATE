# https://wiki.osdev.org/Interrupts_Tutorial
# http://www.osdever.net/bkerndev/Docs/irqs.htm

.intel_syntax noprefix

.macro irq_stub n, k
.global irq_stub_\n
irq_stub_\n:
    push 0
    push \k
    jmp irq_common_stub
.endm

.extern irq_handler
.global irq_common_stub
irq_common_stub:
    pusha

    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp

    push eax

    call irq_handler

    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa

    add esp, 8
    iret

// https://wiki.osdev.org/Interrupts#General_IBM-PC_Compatible_Interrupt_Information
irq_stub 0, 32 // Programmable Interrupt Timer Interrupt
irq_stub 1, 33 // Keyboard Interrupt
irq_stub 2, 34 // Cascade
irq_stub 3, 35 // COM2
irq_stub 4, 36 // COM1
irq_stub 5, 37 // LPT2
irq_stub 6, 38 // Floppy Disk
irq_stub 7, 39 // LPT1
irq_stub 8, 40 // CMOS real-time clock
irq_stub 9, 41 // Free for peripherals / legacy SCSI / NIC
irq_stub 10, 42 // Free for peripherals / SCSI / NIC
irq_stub 11, 43 // Free for peripherals / SCSI / NIC
irq_stub 12, 44 // PS2 Mouse
irq_stub 13, 45 // FPU / Coprocessor / Inter-processor
irq_stub 14, 46 // Primary ATA Hard Disk
irq_stub 15, 47 // Secondary ATA Hard Disk

.global irq_table
irq_table:
    .long irq_stub_0
    .long irq_stub_1
    .long irq_stub_2
    .long irq_stub_3
    .long irq_stub_4
    .long irq_stub_5
    .long irq_stub_6
    .long irq_stub_7
    .long irq_stub_8
    .long irq_stub_9
    .long irq_stub_10
    .long irq_stub_11
    .long irq_stub_12
    .long irq_stub_13
    .long irq_stub_14
    .long irq_stub_15

