.intel_syntax noprefix
.section .data

// the assembler doesn't seem to have support for macros?
.global isr_table
isr_table:
   .long isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7
   .long isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15
   .long isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23
   .long isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31

.global isr0
isr0:
   cli
   hlt

.global isr1
isr1:
   cli
   hlt

.global isr2
isr2:
   cli
   hlt

.global isr3
isr3:
   cli
   hlt

.global isr4
isr4:
   cli
   hlt

.global isr5
isr5:
   cli
   hlt

.global isr6
isr6:
   cli
   hlt

.global isr7
isr7:
   cli
   hlt

.global isr8
isr8:
   cli
   hlt

.global isr9
isr9:
   cli
   hlt

.global isr10
isr10:
   cli
   hlt

.global isr11
isr11:
   cli
   hlt

.global isr12
isr12:
   cli
   hlt

.global isr13
isr13:
   cli
   hlt

.global isr14
isr14:
   cli
   hlt

.global isr15
isr15:
   cli
   hlt

.global isr16
isr16:
   cli
   hlt

.global isr17
isr17:
   cli
   hlt

.global isr18
isr18:
   cli
   hlt

.global isr19
isr19:
   cli
   hlt

.global isr20
isr20:
   cli
   hlt

.global isr21
isr21:
   cli
   hlt

.global isr22
isr22:
   cli
   hlt

.global isr23
isr23:
   cli
   hlt

.global isr24
isr24:
   cli
   hlt

.global isr25
isr25:
   cli
   hlt

.global isr26
isr26:
   cli
   hlt

.global isr27
isr27:
   cli
   hlt

.global isr28
isr28:
   cli
   hlt

.global isr29
isr29:
   cli
   hlt

.global isr30
isr30:
   cli
   hlt

.global isr31
isr31:
   cli
   hlt
