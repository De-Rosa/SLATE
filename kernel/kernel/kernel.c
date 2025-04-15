#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/ps2.h>

#include <kernel/memory/pmem.h>
#include <kernel/memory/vmem.h>

void kernel_main(void) {
	terminal_initialize();

	setup_gdt();
	setup_idt();

	// Occasionally causes a general fault protection exception?
	initialise_controller();

	// test interrupts!
	volatile int z = 1 / 0;

	printf("Welcome to SLATE!\n");
}
