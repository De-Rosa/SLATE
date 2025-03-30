.intel_syntax noprefix
.section .data
idtr:
   .word 0
   .long 0

.section .text
.global setIDT
setIDT:
   MOV   ax, [esp + 4] // get limit 16 bytes
   MOV   [idtr], ax // put it in IDTR
   MOV   eax, [esp + 8] // get base 32 bytes
   MOV   [idtr + 2], eax // put it in IDTR
   LIDT  [idtr] // load the GDT
   RET
