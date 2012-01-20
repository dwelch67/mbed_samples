
#include "parity.h"

extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET8 ( unsigned int );
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );
extern void ASMDELAY ( unsigned int );
extern unsigned int PUTGETCLR ( unsigned int, unsigned int );
extern unsigned int PUTGETSET ( unsigned int, unsigned int );

#define GPIO_BASE 0x50000000

#define GPIO_W00 (GPIO_BASE+0x1000)
#define GPIO_W01 (GPIO_BASE+0x1004)
#define GPIO_W02 (GPIO_BASE+0x1008)
#define GPIO_W03 (GPIO_BASE+0x100C)
#define GPIO_W04 (GPIO_BASE+0x1010)
#define GPIO_W05 (GPIO_BASE+0x1014)
#define GPIO_W06 (GPIO_BASE+0x1018)
#define GPIO_W07 (GPIO_BASE+0x101C)

#define GPIO_DIR0 (GPIO_BASE+0x2000)
#define GPIO_PIN0 (GPIO_BASE+0x2100)
#define GPIO_SET0 (GPIO_BASE+0x2200)
#define GPIO_CLR0 (GPIO_BASE+0x2280)

#define GPIO1_B00 (GPIO_BASE+0x20)
#define GPIO1_B08 (GPIO_BASE+0x28)
#define GPIO1_B09 (GPIO_BASE+0x29)
#define GPIO1_B10 (GPIO_BASE+0x2A)
#define GPIO1_B11 (GPIO_BASE+0x2B)

#define GPIO1_B16 (GPIO_BASE+0x30)
#define GPIO1_B17 (GPIO_BASE+0x31)
#define GPIO1_B18 (GPIO_BASE+0x32)


#define GPIO_DIR1 (GPIO_BASE+0x2004)
#define GPIO_SET1 (GPIO_BASE+0x2204)
#define GPIO_CLR1 (GPIO_BASE+0x2284)

#define STCTRL   0xE000E010
#define STRELOAD 0xE000E014
#define STCURR   0xE000E018


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

//0.18 RXD 0.19 TXD
#define PIO0_18 0x40044048
#define PIO0_19 0x4004404C
#define UARTCLKDIV 0x40048098


#define PDRUNCFG        0x40048238
#define SYSOSCCTRL      0x40048020
#define SYSPLLCLKSEL    0x40048040
#define SYSPLLCLKUEN    0x40048044
#define MAINCLKSEL      0x40048070
#define MAINCLKUEN      0x40048074
#define SYSAHBCLKCTRL   0x40048080
#define SYSAHBCLKDIV    0x40048078
#define SYSPLLCTRL      0x40048008
#define SYSPLLSTAT      0x4004800C


#define PIO0_7          0x4004401C
#define PIO1_17         0x400440A4
#define PIO1_18         0x400440A8

int clock_init ( void )
{
    unsigned int ra;

    if(1)
    {
        //A little safety measure.  At the time of this writing it is
        //easy to hang the ARM if you make a mistake configuring the
        //clocks such that the mbed interface chip cannot re-program it
        //(for example to remove the code in flash you used to hang it)
        //One solution is to put a jumper to ground on pin 5 of the
        //microcontroller, causing the microcontroller to boot in ISP
        //mode, basically from a different program in flash which does
        //not hang the arm.  This pin is hard to get to on the mbed but
        //GPIO 0.7 is easy to get to.  If there is a jumper between the
        //two corners, ground and PIN21 then this function will abort
        //and not configure the clock.  This gives you a little more
        //freedom to mess with the clock init code and not have the
        //recovery as painful.

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
void uart_init_48_to_12 ( void )
{
    PUTGETSET(SYSAHBCLKCTRL,(1<<16)|(1<<12)|(1<<6));
    //p0.18 and 0.19 are tied to the mbed interface and then on to
    // /dev/ttyACM# on the host
    PUT32(PIO0_18,0x00000001); //use alternate function RXD for p0.18
    PUT32(PIO0_19,0x00000001); //use alternate function TXD for p0.19
    PUT32(UARTCLKDIV,4); //divide 48/4 = 12 and use same init as 12MHz
//12000000 Hz PCLK 115200 baud
//dl 0x04 mul 0x08 div 0x05 baud 115385 diff 185
    PUT32(U0ACR,0x00); //no autobaud
    PUT32(U0LCR,0x83); //dlab=1; N81
    PUT32(U0DLL,0x04); //dl = 0x0004
    PUT32(U0DLM,0x00); //dl = 0x0004
    PUT32(U0IER,0x00); //no interrupts
    PUT32(U0LCR,0x03); //dlab=0; N81
    PUT32(U0IER,0x00); //no interrupts
    PUT32(U0FDR,(0x8<<4)|(0x5<<0)); //mul 0x08, div 0x05
    PUT32(U0FCR,(1<<2)|(1<<1)|(1<<0)); //enable and reset fifos
    PUT32(U0TER,(1<<7)); //transmit enable
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
//48000000 Hz PCLK 115200 baud
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
void hexstring ( unsigned int d, unsigned int cr )
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
    if(cr)
    {
        uart_putc(0x0D);
        uart_putc(0x0A);
    }
    else
    {
        uart_putc(0x20);
    }
}
//-------------------------------------------------------------------
void dowait ( void )
{
    unsigned int ra;

    ra=uart_getc();
    uart_putc(ra);
    if(ra==0x0D) uart_putc(0x0A);
}
    unsigned int pdi_data_in,pdi_data_out;
#define PDI_DIR_OUT PUT32(GPIO_DIR1,pdi_data_out)
#define PDI_DIR_IN  PUT32(GPIO_DIR1,pdi_data_in)
#define SET_PDI_DATA PUT8(GPIO1_B17,1)
#define CLR_PDI_DATA PUT8(GPIO1_B17,0)
#define SET_PDI_CLK  PUT8(GPIO1_B18,1)
#define CLR_PDI_CLK  PUT8(GPIO1_B18,0)
#define DELX 10
//-------------------------------------------------------------------
void send_pdi_command ( unsigned int x )
{
    unsigned int ra;
    unsigned int rb;

    //109876543210
    //xxp76543210s

    ra=0xFFFFFFFF;
    ra<<=1;
    if(parity[x&0xFF]) ra|=1;
    ra<<=8;
    ra|=x;
    ra<<=1;

    PDI_DIR_OUT;
    for(rb=0;rb<12;rb++)
    {
        CLR_PDI_CLK;
        PUT8(GPIO1_B17,ra&1);
        ASMDELAY(DELX);

        SET_PDI_CLK;
        ra>>=1;
        ASMDELAY(DELX);
    }
}
//-------------------------------------------------------------------
void send_pdi_break ( unsigned int len )
{
    unsigned int ra;

    PDI_DIR_OUT;
    CLR_PDI_DATA;
    for(ra=0;ra<len;ra++)
    {
        CLR_PDI_CLK;
        ASMDELAY(DELX);
        SET_PDI_CLK;
        ASMDELAY(DELX);
    }

}
//-------------------------------------------------------------------
void send_pdi_idle ( unsigned int len )
{
    unsigned int ra;

    PDI_DIR_OUT;
    SET_PDI_DATA;
    for(ra=0;ra<len;ra++)
    {
        CLR_PDI_CLK;
        ASMDELAY(DELX);
        SET_PDI_CLK;
        ASMDELAY(DELX);
    }

}
//-------------------------------------------------------------------
void notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;



    clock_init();
    uart_init();
    //uart_init_48_to_12();
    hexstring(0x12345678,1);

    ra=GET32(GPIO_DIR1);
    ra|=1<<8;
    ra|=1<<9;
    ra|=1<<10;
    ra|=1<<11;
    ra|=1<<17; //pdi_data will be in and out
    ra|=1<<18; //pdi_clock will be out only
    PUT32(GPIO_DIR1,ra);

    PUT32(STCTRL,0x00000004); //disabled, no ints, use cpu clock
    PUT32(STRELOAD,0xFFFFFF);
    PUT32(STCTRL,0x00000005); //enabled, no ints, use cpu clock


    PUT32(PIO1_17,0x00000000);
    PUT32(PIO1_18,0x00000000);

    pdi_data_in=GET32(GPIO_DIR1);
    pdi_data_in&=~(1<<17);
    pdi_data_out=pdi_data_in|(1<<17);



    PDI_DIR_OUT;
    SET_PDI_CLK;
   // CLR_PDI_DATA;
    //ASMDELAY(2000);
    SET_PDI_DATA;
    ASMDELAY(500000);
    send_pdi_idle(200);


    send_pdi_break(24);
    send_pdi_idle(24);

    //set address
    //send_pdi_command(0x6B);
    //send_pdi_command(0x00);
    //send_pdi_command(0x00);
    //send_pdi_command(0x20);
    //send_pdi_command(0x00);

    //send_pdi_command(0x65);
    //send_pdi_command(0x12);
    //send_pdi_command(0x34);

    //send_pdi_command(0x6B);
    //send_pdi_command(0x00);
    //send_pdi_command(0x00);
    //send_pdi_command(0x20);
    //send_pdi_command(0x00);

    //send_pdi_command(0x25);

    send_pdi_command(0x6B);
    send_pdi_command(0x12);
    send_pdi_command(0x34);
    send_pdi_command(0x56);
    send_pdi_command(0x78);
    send_pdi_command(0x2B);

  //  send_pdi_idle(12);

    PDI_DIR_IN;
    while(1)
    {
        CLR_PDI_CLK;
        ASMDELAY(DELX);
        SET_PDI_CLK;
        ra=GET8(GPIO1_B17)&1;
        ASMDELAY(DELX);
        if(ra==0) break;
    }
    rc=0;
    for(rb=0;rb<30;rb++)
    {
        CLR_PDI_CLK;
        ASMDELAY(DELX);
        SET_PDI_CLK;
        ra=GET8(GPIO1_B17)&1;
        ASMDELAY(DELX);
        rc<<=1;
        rc|=ra;
    }
    hexstring(rc,1);





}
