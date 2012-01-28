
.org 0x0000
    rjmp RESET

RESET:


;;
;;
;;    ldi r19,0x02
;;    ldi r30,0x50
;;    ldi r31,0x00
;;    st z+,r19       ;; enable 32MHz internal oscillator
;;osc_wait:
;;    ld r18,z
;;    andi r18,0x2
;;    breq osc_wait   ;; wait for it to be stable
;;    ldi r18,0xD8
;;    ldi r19,0x01
;;
;;    ldi r28,0x34
;;    ldi r29,0x00
;;
;;    ldi r30,0x40
;;    ldi r31,0x00
;;
;;    st y,r18        ;; ccp I/O protection
;;    st z,r19        ;; change clock source
;;    nop
;;    nop
;;    nop
;;
;;#define SPL  (0x3D + 0x20)
;;#define SPH  (0x3E + 0x20)



    ldi r30,0x40
    ldi r31,0x20

    ldi r16,0x69
    st z,r16

    ldi r16,0x3C
    mov r0,r16
    ;lac z,rd
    .dw 0x9204



    ldi R16,0xFF
    sts 0x6A1,R16

    ld r24,z
    mov r24,r0
    ;mov r24,r30

    mov r25,r24
    andi r25,0x80
    rcall dobit
    mov r25,r24
    andi r25,0x40
    rcall dobit
    mov r25,r24
    andi r25,0x20
    rcall dobit
    mov r25,r24
    andi r25,0x10
    rcall dobit
    mov r25,r24
    andi r25,0x08
    rcall dobit
    mov r25,r24
    andi r25,0x04
    rcall dobit
    mov r25,r24
    andi r25,0x02
    rcall dobit
    mov r25,r24
    andi r25,0x01
    rcall dobit

top:
    rjmp top





dobit:
    breq zero
    ;brne one


one:
    ldi R16,0xFF
    sts 0x6A4,R16

     ldi R18,0x00
     ldi R17,0x00
     ldi R19,0x10
oloop:
     inc R17
     cpi R17,0x00
     brne oloop

     inc R18
     cpi R18,0x00
     brne oloop

     dec R19
     cpi R19,0x00
     brne oloop

     rjmp off


zero:
    ldi R16,0xFF
    sts 0x6A4,R16

     ldi R18,0x00
     ldi R17,0x00
     ldi R19,0x4
zloop:
     inc R17
     cpi R17,0x00
     brne zloop

     inc R18
     cpi R18,0x00
     brne zloop

     dec R19
     cpi R19,0x00
     brne zloop

     rjmp off

off:
    ldi R16,0x00
    sts 0x6A4,R16

     ldi R18,0x00
     ldi R17,0x00
     ldi R19,0x4
floop:
     inc R17
     cpi R17,0x00
     brne floop

     inc R18
     cpi R18,0x00
     brne floop

     dec R19
     cpi R19,0x00
     brne floop

     ret

