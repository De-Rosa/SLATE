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

	// Causes a general fault protection exception
	// initialise_controller();

	printf("Welcome to SLATE!\n");
}
