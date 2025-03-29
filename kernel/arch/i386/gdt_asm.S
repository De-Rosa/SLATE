.intel_syntax noprefix
.section .data
gdtr:
   .word 0
   .long 0

.section .text
.global setGDT
setGDT:
   MOV   ax, [esp + 4] // get limit 16 bytes
   MOV   [gdtr], ax // put it in GDTR
   MOV   eax, [esp + 8] // get base 32 bytes
   MOV   [gdtr + 2], eax // put it in GDTR
   LGDT  [gdtr] // load the GDT
   RET

.global reloadSegments
reloadSegments:
   JMP   0x08:.reload_CS 
.reload_CS:
   MOV   AX, 0x10 
   MOV   DS, AX
   MOV   ES, AX
   MOV   FS, AX
   MOV   GS, AX
   MOV   SS, AX
   RET
