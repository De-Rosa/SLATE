// http://www.osdever.net/bkerndev/Docs/isrs.htm

#include <kernel/interrupt/idt.h>
#include <kernel/tty.h>

const char* exception_descriptions[] = {
    "Division By Zero Exception",
    "Debug Exception",
    "Non Maskable Interrupt Exception",
    "Breakpoint Exception",
    "Into Detected Overflow Exception",
    "Out of Bounds Exception",
    "Invalid Opcode Exception",
    "No Coprocessor Exception",
    "Double Fault Exception",
    "Coprocessor Segment Overrun Exception",
    "Bad TSS Exception",
    "Segment Not Present Exception",
    "Stack Fault Exception",
    "General Protection Fault Exception",
    "Page Fault Exception",
    "Unknown Interrupt Exception",
    "Coprocessor Fault Exception",
    "Alignment Check Exception",
    "Machine Check Exception"
};

void exception_handler(struct registers* regs) {
	if (regs->int_no > 32) return;

	terminal_error("Exception handler called!\n");
	terminal_error(exception_descriptions[regs->int_no]);

	__asm__ volatile ("cli; hlt"); // hang the system
}
