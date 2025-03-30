// https://wiki.osdev.org/Interrupts_Tutorial

__attribute__((noreturn))
void exception_handler(void);

void exception_handler() {
	__asm__ volatile ("cli; hlt");
}
