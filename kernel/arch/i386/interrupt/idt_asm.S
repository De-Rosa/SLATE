.intel_syntax noprefix
.section .data
idtr:
   .word 0
   .long 0

.section .text
.global setIDT
setIDT:
   mov   ax, [esp + 4] // get limit 16 bits
   mov   [idtr], ax // put it in idtr
   mov   eax, [esp + 8] // get base 32 bits
   mov   [idtr + 2], eax // put it in idtr
   lidt  [idtr] // load the gdt
   sti
   ret

.global outb
// https://c9x.me/x86/html/file_module_x86_id_222.html
outb:
    mov dx, [esp + 4]   // load port into dx
    mov al, [esp + 8]   // load operand into al
    out dx, al
    ret

.global inb
// https://c9x.me/x86/html/file_module_x86_id_139.html
inb:
   mov dx, [esp + 4] // load port into dx
   in al, dx
   ret
