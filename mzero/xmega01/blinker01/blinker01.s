
.org 0x0000
    rjmp RESET

RESET:
    ldi R16,0xFF
    ldi R21,0xFF
    sts 0x6A1,R16

    ldi R18,0x00
    ldi R17,0x00
    ldi R20,0x20
Loop:

    ldi R19,0xE8
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

