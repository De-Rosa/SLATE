.intel_syntax noprefix
.section .data
gdtr:
   .word 0
   .long 0

.section .text
.global setGDT
setGDT:
   mov   ax, [esp + 4] // get limit 16 bits
   mov   [gdtr], ax // put it in GDTR
   mov   eax, [esp + 8] // get base 32 bits
   mov   [gdtr + 2], eax // put it in GDTR
   lgdt  [gdtr] // load the GDT
   ret

.global setTSS
setTSS:
   mov   ax, 0x28
   ltr   ax
   ret

.global reloadSegments
reloadSegments:
   jmp   0x08:.reload_CS 
.reload_CS:
   mov   ax, 0x10 
   mov   ds, ax
   mov   es, ax
   mov   fs, ax
   mov   gs, ax
   mov   ss, ax
   ret
