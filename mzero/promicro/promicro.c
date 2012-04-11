
//-------------------------------------------------------------------
//-------------------------------------------------------------------

#include "blinker.h"

extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET8 ( unsigned int );
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );
extern void ASMDELAY ( unsigned int );
extern unsigned int PUTGETCLR ( unsigned int, unsigned int );
extern unsigned int PUTGETSET ( unsigned int, unsigned int );

#define GPIO_BASE 0x50000000

//#define GPIO_W00 (GPIO_BASE+0x1000)
//#define GPIO_W01 (GPIO_BASE+0x1004)
//#define GPIO_W02 (GPIO_BASE+0x1008)
//#define GPIO_W03 (GPIO_BASE+0x100C)
//#define GPIO_W04 (GPIO_BASE+0x1010)
//#define GPIO_W05 (GPIO_BASE+0x1014)
//#define GPIO_W06 (GPIO_BASE+0x1018)
#define GPIO_W07 (GPIO_BASE+0x101C)

#define GPIO_DIR0 (GPIO_BASE+0x2000)
//#define GPIO_PIN0 (GPIO_BASE+0x2100)
//#define GPIO_SET0 (GPIO_BASE+0x2200)
//#define GPIO_CLR0 (GPIO_BASE+0x2280)

//#define GPIO1_B00 (GPIO_BASE+0x20)
//#define GPIO1_B08 (GPIO_BASE+0x28)
//#define GPIO1_B09 (GPIO_BASE+0x29)
//#define GPIO1_B10 (GPIO_BASE+0x2A)
//#define GPIO1_B11 (GPIO_BASE+0x2B)

//#define GPIO1_B16 (GPIO_BASE+0x30)
#define GPIO1_B17 (GPIO_BASE+0x31)
#define GPIO1_B18 (GPIO_BASE+0x32)
//#define GPIO1_B19 (GPIO_BASE+0x33)
//#define GPIO1_B20 (GPIO_BASE+0x34)
//#define GPIO1_B21 (GPIO_BASE+0x35)
//#define GPIO1_B22 (GPIO_BASE+0x36)
//#define GPIO1_B23 (GPIO_BASE+0x37)
#define GPIO1_B24 (GPIO_BASE+0x38)
#define GPIO1_B25 (GPIO_BASE+0x39)


#define GPIO_DIR1 (GPIO_BASE+0x2004)
//#define GPIO_SET1 (GPIO_BASE+0x2204)
//#define GPIO_CLR1 (GPIO_BASE+0x2284)

//#define STCTRL   0xE000E010
//#define STRELOAD 0xE000E014
//#define STCURR   0xE000E018


#define U0ACR       0x40008020
#define U0LCR       0x4000800C
#define U0DLL       0x40008000
#define U0DLM       0x40008004
#define U0IER       0x40008004
#define U0FDR       0x40008028
#define U0FCR       0x40008008
#define U0TER       0x40008030
#define U0LSR       0x40008014
#define U0THR       0x40008000
#define U0RBR       0x40008000

#define SYSAHBCLKCTRL  0x40048080

////0.18 RXD 0.19 TXD
#define PIO0_18 0x40044048
#define PIO0_19 0x4004404C
#define UARTCLKDIV 0x40048098


#define PDRUNCFG        0x40048238
#define SYSOSCCTRL      0x40048020
#define SYSPLLCLKSEL    0x40048040
#define SYSPLLCLKUEN    0x40048044
#define MAINCLKSEL      0x40048070
#define MAINCLKUEN      0x40048074
//#define SYSAHBCLKCTRL   0x40048080
#define SYSAHBCLKDIV    0x40048078
#define SYSPLLCTRL      0x40048008
#define SYSPLLSTAT      0x4004800C


#define PIO0_7          0x4004401C
#define PIO1_17         0x400440A4
#define PIO1_18         0x400440A8
#define PIO1_25         0x400440C0
#define PIO1_26         0x400440C4

//-------------------------------------------------------------------
int clock_init ( void )
{
    unsigned int ra;

    if(1)
    {
        // A little safety measure.

        //if GPIO 0.7 is shorted to ground, then bypass clock config
        PUTGETSET(SYSAHBCLKCTRL,(1<<16)); //need this for I/O in general
        PUTGETSET(SYSAHBCLKCTRL,(1<<6)); //specifically make sure gpio is enabled
        PUT32(PIO0_7,0x00000010); //use it as a GPIO with a pull up resistor
        PUTGETCLR(GPIO_DIR0,1<<7); //make it an input
        for(ra=0;ra<200;ra++) dummy(0); //just in case it needs to settle
        if(GET32(GPIO_W07)==0xFFFFFFFF)
        {
            //keep going
        }
        else
        {
            //do not re-configure the clocks
            return(1);
        }
    }

    PUTGETCLR(PDRUNCFG,1<<5); //power up system oscillator
    PUT32(SYSOSCCTRL,0x00000000);
    for(ra=0;ra<200;ra++) dummy(0); //kill some time for it to come up

    PUT32(SYSPLLCLKSEL,0x00000001); //select crystal oscillator as pll input
    PUT32(SYSPLLCLKUEN,0x00000001);
    PUT32(SYSPLLCLKUEN,0x00000000);
    PUT32(SYSPLLCLKUEN,0x00000001);
    while((GET32(SYSPLLCLKUEN)&0x00000001)==0) continue; //wait for it to become the pll input
    //if you are using the pll
    if(1)
    {
        //M = 4 (3+1), P = 2 (1*2)  P is for the PLL not a divisor of the
        //main clock.  So this results in 12 * 4 / SYSAHBCLKDIV
        PUT32(SYSPLLCTRL,0x00000023);
        PUTGETCLR(PDRUNCFG,1<<7); //power up system pll
        while((GET32(SYSPLLSTAT)&0x00000001)==0) continue; //wait for PLL to lock
    }

    PUT32(MAINCLKSEL,0x00000003); //select main clock source, pll output
    PUT32(MAINCLKUEN,0x00000001);
    PUT32(MAINCLKUEN,0x00000000);
    PUT32(MAINCLKUEN,0x00000001);
    while((GET32(MAINCLKUEN)&0x00000001)==0) continue; //wait for it

    PUT32(SYSAHBCLKDIV,0x00000001); //divide by 1 gives us 48MHz

    //not using usb
    PUTGETSET(PDRUNCFG,(1<<10));
    PUTGETSET(PDRUNCFG,(1<<8));

    PUTGETSET(SYSAHBCLKCTRL,(1<<16)); //need this for I/O in general
    PUTGETSET(SYSAHBCLKCTRL,(1<<6)); //specifically make sure gpio is enabled
    return(0);
}
//-------------------------------------------------------------------
void uart_init ( void )
{
    PUTGETSET(SYSAHBCLKCTRL,(1<<16)|(1<<12)|(1<<6));
    //p0.18 and 0.19 are tied to the mbed interface and then on to
    // /dev/ttyACM# on the host
    PUT32(PIO0_18,0x00000001); //use alternate function RXD for p0.18
    PUT32(PIO0_19,0x00000001); //use alternate function TXD for p0.19
    PUT32(UARTCLKDIV,1);
//48200000 Hz PCLK 115200 baud
//dl 0x11 mul 0x0F div 0x08 baud 115089 diff 89
    PUT32(U0ACR,0x00); //no autobaud
    PUT32(U0LCR,0x83); //dlab=1; N81
    PUT32(U0DLL,0x11); //dl = 0x0011
    PUT32(U0DLM,0x00); //dl = 0x0011
    PUT32(U0IER,0x00); //no interrupts
    PUT32(U0LCR,0x03); //dlab=0; N81
    PUT32(U0IER,0x00); //no interrupts
    PUT32(U0FDR,(0xF<<4)|(0x8<<0)); //mul 0x0F, div 0x08
    PUT32(U0FCR,(1<<2)|(1<<1)|(1<<0)); //enable and reset fifos
    PUT32(U0TER,(1<<7)); //transmit enable
}
//-------------------------------------------------------------------
void uart_putc ( unsigned int x )
{
    while (( GET32(U0LSR) & (1<<5)) == 0) continue;
    PUT32(U0THR,x);
}
//-------------------------------------------------------------------
unsigned int uart_getc ( void )
{
    while (( GET32(U0LSR) & (1<<0)) == 0) continue;
    return(GET32(U0RBR));
}
//-------------------------------------------------------------------
void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_putc(rc);
        if(rb==0) break;
    }
    uart_putc(0x20);
}
//-------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_putc(0x0D);
    uart_putc(0x0A);
}
//-------------------------------------------------------------------

#define ARD_RST_PIN 25
#define ARD_SCK_PIN 24
#define ARD_PDO_PIN 18
#define ARD_PDI_PIN 17

#define ARD_RST_REG GPIO1_B25
#define ARD_SCK_REG GPIO1_B24
#define ARD_PDO_REG GPIO1_B18
#define ARD_PDI_REG GPIO1_B17


unsigned int pdi_data_in,pdi_data_out;
#define SET_RST PUT8(ARD_RST_REG,1)
#define CLR_RST PUT8(ARD_RST_REG,0)
#define SET_PDI PUT8(ARD_PDI_REG,1)
#define CLR_PDI PUT8(ARD_PDI_REG,0)
#define SET_PDO PUT8(ARD_PDO_REG,1)
#define CLR_PDO PUT8(ARD_PDO_REG,0)
#define SET_SCK PUT8(ARD_SCK_REG,1)
#define CLR_SCK PUT8(ARD_SCK_REG,0)

#define DELX 20

//-------------------------------------------------------------------
unsigned int pdi_command ( unsigned int command )
{
    unsigned int ra,rb,rc;

    //hexstring(command);
    rb=command;
    rc=0x00000000;
    //CLR_SCK;
    ASMDELAY(DELX);
    for(ra=0;ra<32;ra++)
    {
        PUT8(ARD_PDI_REG,(rb>>31)&1); rb<<=1;
        rc<<=1; rc|=GET8(ARD_PDO_REG)&1;
        ASMDELAY(DELX);
        SET_SCK;
        ASMDELAY(DELX);
        ASMDELAY(DELX);
        CLR_SCK;
        ASMDELAY(DELX);
    }
    ASMDELAY(DELX);
    //hexstring(rc);
    return(rc);
}
//-------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    unsigned int pages;
    unsigned int psize;
    //unsigned int np;
    unsigned int poff;

    ra=0;

    clock_init();
    uart_init();
    //uart_init_48_to_12();
    hexstring(0x12345678);

    //start by not driving against the avr pins except reset
    ra=GET32(GPIO_DIR1);
    ra|=1<<8;
    ra|=1<<9;
    ra|=1<<10;
    ra|=1<<11;
    ra&=~(1<<ARD_PDO_PIN);
    ra|=1<<ARD_PDI_PIN;
    ra|=1<<ARD_SCK_PIN;
    ra|=1<<ARD_RST_PIN;
    PUT32(GPIO_DIR1,ra);

    PUT32(PIO1_17,0x00000000);
    PUT32(PIO1_18,0x00000000);

    PUT32(PIO1_25,0x00000000);
    PUT32(PIO1_26,0x00000000);

    CLR_SCK;
    CLR_RST;

    //seems like you bounce or get more than one mbed reset
    //put a huge delay in so we dont start hitting the other part a
    //number of times.
    ASMDELAY(500000);

    SET_RST;
    ASMDELAY(100);
    CLR_RST;
    ASMDELAY(100);


    ra=pdi_command(0xAC530000);
    if((ra&0x0000FF00)!=0x00005300)
    {
        hexstring(0xBAD);
        return(1);
    }
    rb=0;
    ra=pdi_command(0x30000000);
    rb<<=8; rb|=ra&0xFF;
    ra=pdi_command(0x30000100);
    rb<<=8; rb|=ra&0xFF;
    ra=pdi_command(0x30000200);
    rb<<=8; rb|=ra&0xFF;
    hexstring(rb);
    if(rb!=0x001E9587)
    {
        //not really an error, this code is written for the atmega32u4
        //should be easy to adapt to another part.
        hexstring(0xBAD);
        return(1);
    }
    for(ra=0;ra<10;ra++)
    {
        rb=pdi_command(0x28000000|(ra<<8));
        rc=pdi_command(0x20000000|(ra<<8));
        hexstring((ra<<16)|((rb&0xFF)<<8)|(rc&0xFF));
    }


    pdi_command(0xAC800000);
    SET_RST;

    ASMDELAY(100000);
    CLR_RST;
    ASMDELAY(100);

    ra=pdi_command(0xAC530000);
    if((ra&0x0000FF00)!=0x00005300)
    {
        hexstring(0xBAD);
        return(1);
    }
    rb=0;
    ra=pdi_command(0x30000000);
    rb<<=8; rb|=ra&0xFF;
    ra=pdi_command(0x30000100);
    rb<<=8; rb|=ra&0xFF;
    ra=pdi_command(0x30000200);
    rb<<=8; rb|=ra&0xFF;
    hexstring(rb);
    if(rb!=0x001E9587)
    {
        //not really an error, this code is written for the atmega32u4
        //should be easy to adapt to another part.
        hexstring(0xBAD);
        return(1);
    }
    for(ra=0;ra<10;ra++)
    {
        rb=pdi_command(0x28000000|(ra<<8));
        rc=pdi_command(0x20000000|(ra<<8));
        hexstring((ra<<16)|((rb&0xFF)<<8)|(rc&0xFF));
    }

    psize=sizeof(rom);
    psize>>=1;
    pages=psize>>6;

    hexstring(psize);
    hexstring(pages);

    poff=0;
    //for(np=0;np<pages;np++)
    {
        for(ra=0;ra<64;ra++)
        {
//            if(poff>=psize) break;
            pdi_command(0x40000000|(ra<<8)|((rom[poff]>>0)&0xFF)); //low first
            pdi_command(0x48000000|(ra<<8)|((rom[poff]>>8)&0xFF)); //then high
            poff++;
        }
        //for(;ra<64;ra++)
        //{
            //pdi_command(0x400000FF|(ra<<8)); //low first
            //pdi_command(0x480000FF|(ra<<8)); //then high
            ////poff++;
        //}
        pdi_command(0x4D000000);
        pdi_command(0x4C000000);//|(np<<14));
        ASMDELAY(100000);
    }
    for(ra=0;ra<10;ra++)
    {
        rb=pdi_command(0x28000000|(ra<<8));
        rc=pdi_command(0x20000000|(ra<<8));
        hexstring((ra<<16)|((rb&0xFF)<<8)|(rc&0xFF));
    }

    ra=pdi_command(0x50000000); hexstring(ra);
    ra=pdi_command(0x58080000); hexstring(ra);
    ra=pdi_command(0x50080000); hexstring(ra);

    SET_RST;
    hexstring(poff);

    ra=GET32(GPIO_DIR1);
    ra&=~(1<<ARD_PDI_PIN);
    ra&=~(1<<ARD_SCK_PIN);
    PUT32(GPIO_DIR1,ra);

    hexstring(0xAABBCCDD);
    hexstring(0x12345678);
    return(0);
}
