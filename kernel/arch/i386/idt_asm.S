.intel_syntax noprefix
.section .data
idtr:
   .word 0
   .long 0

.section .text
.global setIDT
setIDT:
   MOV   ax, [esp + 4] // get limit 16 bits
   MOV   [idtr], ax // put it in IDTR
   MOV   eax, [esp + 8] // get base 32 bits
   MOV   [idtr + 2], eax // put it in IDTR
   LIDT  [idtr] // load the GDT
   STI
   RET

.global outb
// https://c9x.me/x86/html/file_module_x86_id_222.html
outb:
   MOV AX, [ESP + 6]    // get port 16 bits
   MOV AL, [ESP + 4]    // get operand 8 bits
   OUT DX, AL
   RET
