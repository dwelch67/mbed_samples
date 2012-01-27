
.org 0x0000
    rjmp RESET

RESET:

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

    ldi R16,0xFF
    ldi R21,0xFF
    sts 0x6A1,R16
    sts 0x6A4,R16


     ldi R18,0x00
     ldi R17,0x00
     ldi R20,0x20
 Loop:

     ldi R19,0xD0
 aloop:
     inc R17
     cpi R17,0x00
     brne aloop

     inc R18
     cpi R18,0x00
     brne aloop

     inc R19
     cpi R19,0x00
     brne aloop

     eor R16,R21
     sts 0x6A4, R16
     rjmp Loop

