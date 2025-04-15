.intel_syntax noprefix
.section .data
gdtr:
   .word 0
   .long 0

.section .text
.global setGDT
setGDT:
   MOV   ax, [esp + 4] // get limit 16 bits
   MOV   [gdtr], ax // put it in GDTR
   MOV   eax, [esp + 8] // get base 32 bits
   MOV   [gdtr + 2], eax // put it in GDTR
   LGDT  [gdtr] // load the GDT
   RET

.global reloadSegments
reloadSegments:
   JMP   0x08:.reload_CS 
.reload_CS:
   MOV   ax, 0x10 
   MOV   ds, ax
   MOV   es, ax
   MOV   fs, ax
   MOV   gs, ax
   MOV   ss, ax
   RET
