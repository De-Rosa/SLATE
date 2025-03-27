# https://wiki.osdev.org/GDT_Tutorial#Telling_the_CPU_Where_the_Table_Is
# Protected Mode, Flat Model

gdtr DW 0 ;
     DD 0 ;

setGDT:
   MOV   AX, [esp + 4]
   MOV   [gdtr], AX
   MOV   EAX, [ESP + 8]
   MOV   [gdtr + 2], EAX
   LGDT  [gdtr]
   RET
