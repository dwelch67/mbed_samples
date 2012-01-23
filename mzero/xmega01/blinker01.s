
.org 0x0000
    rjmp RESET

RESET:



;;//void setClockTo32MHz() {
;;    //CCP = CCP_IOREG_gc;              // disable register security for oscillator update
;;    //OSC.CTRL = OSC_RC32MEN_bm;       // enable 32MHz oscillator
;;    //while(!(OSC.STATUS & OSC_RC32MRDY_bm)); // wait for oscillator to be ready
;;    //CCP = CCP_IOREG_gc;              // disable register security for clock update
;;    //CLK.CTRL = CLK_SCLKSEL_RC32M_gc; // switch to 32MHz clock
;;//}





    ldi r18,0xD8
    ldi r19,0x02
    sts 0x34,r18
    sts 0x050,r19
osc_wait:
    lds r18,0x051
    sbrs r18,2
    rjmp osc_wait
    ldi r18,0xD8
    ldi r19,0x01
    sts 0x34,r18
    sts 0x040,r19



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



