#ifndef _KERNEL_IRQS_H
#define _KERNEL_IRQS_H

void install_irq_handler(int irq, void (*handler)(struct registers* reg));
void uninstall_irq_handler(int irq);
void install_irqs(void);

#endif
