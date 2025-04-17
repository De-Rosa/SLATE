#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/ps2.h>

#include <kernel/memory/pmem.h>
#include <kernel/memory/vmem.h>

#include <kernel/interrupt/irqs.h>

void kernel_main(void) {
	terminal_initialize();

	setup_gdt();
	setup_idt();

	initialise_controller();
	install_irqs();

	printf("\nWelcome to SLATE!\n");
}
