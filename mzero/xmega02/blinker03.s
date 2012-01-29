
.globl _start
_start:
    rjmp reset


reset:

    ;rjmp over

    ldi r19,0x02
    ldi r30,0x50
    ldi r31,0x00
    st z+,r19       ;; enable 32MHz internal oscillator
osc_wait:
    ld r18,z
    andi r18,0x2
    breq osc_wait   ;; wait for it to be stable
    ldi r18,0xD8
    ldi r19,0x01

    ldi r28,0x34
    ldi r29,0x00

    ldi r30,0x40
    ldi r31,0x00

    st y,r18        ;; ccp I/O protection
    st z,r19        ;; change clock source
    nop
    nop
    nop

over:

    rcall notmain
1:
    rjmp 1b

.globl dummy
dummy:
    ldi r24,0x00
1:
    inc r24
    brne 1b

    ret

