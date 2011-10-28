
/*blinker01.s*/

.cpu cortex-m3
.thumb


.thumb_func
.globl _start
_start:
   ldr r0,=0x2009C040
   ldrb r1,[r0]
   mov r2,#0x01
   orr r1,r2
   strb r1,[r0]

   ldr r0,=0x2009C050
   mov r1,#0x00
   strb r1,[r0]

   ldr r0,=0x2009C058
   ldr r1,=0x2009C05C
   ldr r2,=0x01

mainloop:
   strb r2,[r0]
   bl dowait
   strb r2,[r1]
   bl dowait
   b mainloop

.thumb_func
dowait:
   ldr r7,=0x200000
dowaitloop:
   sub r7,#1
   bne dowaitloop
   bx lr

.end
