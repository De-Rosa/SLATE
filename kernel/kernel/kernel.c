#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>

#include <kernel/memory/pmem.h>
#include <kernel/memory/vmem.h>

void kernel_main(void) {
	terminal_initialize();

	setup_gdt();
	setup_idt();

	printf("Welcome to SLATE!\n");

	// test interrupts!
	volatile int z = 1 / 0;
}
