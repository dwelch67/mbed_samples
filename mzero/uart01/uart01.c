

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

extern unsigned int PUTGETCLR ( unsigned int, unsigned int );
extern unsigned int PUTGETSET ( unsigned int, unsigned int );

#define GPIO_BASE 0x50000000

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
#define PIO0_19 0x4004404C
#define PIO1_27 0x400440CC
#define UARTCLKDIV 0x40048098

//-------------------------------------------------------------------
void uart_init ( void )
{
    PUTGETSET(SYSAHBCLKCTRL,(1<<16)|(1<<12)|(1<<6));
    PUT32(PIO0_19,0x00000001); //goes to interface, seen on /dev/ttyACM#
    //PUT32(PIO1_27,0x00000002); //external pin P9
    PUT32(UARTCLKDIV,1);
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
    ra=1;

    while(ra)
    {
        if(GET32(STCTRL)&0x00010000) ra--;
    }
    uart_putc(0x55);
}
//-------------------------------------------------------------------
void notmain ( void )
{
    unsigned int ra,rb,rc,rd,re,rf;

    uart_init();
    hexstring(0x12345678,1);

    ra=GET32(GPIO_DIR1);
    ra|=1<<8;
    ra|=1<<9;
    ra|=1<<10;
    ra|=1<<11;
    PUT32(GPIO_DIR1,ra);

    PUT32(STCTRL,0x00000004); //disabled, no ints, use cpu clock
    PUT32(STRELOAD,0xFFFFFFF);
    PUT32(STCTRL,0x00000005); //enabled, no ints, use cpu clock


    ra=GPIO_SET1;
    rb=GPIO_CLR1;
    rc=1<<8;
    rd=1<<9;
    re=1<<10;
    rf=1<<11;

    while(1)
    {
        PUT32(ra,rc);
        PUT32(rb,rd);
        PUT32(rb,re);
        PUT32(rb,rf);
        dowait();
        PUT32(rb,rc);
        PUT32(ra,rd);
        PUT32(rb,re);
        PUT32(rb,rf);
        dowait();
        PUT32(rb,rc);
        PUT32(rb,rd);
        PUT32(ra,re);
        PUT32(rb,rf);
        dowait();
        PUT32(rb,rc);
        PUT32(rb,rd);
        PUT32(rb,re);
        PUT32(ra,rf);
        dowait();
        PUT32(rb,rc);
        PUT32(rb,rd);
        PUT32(ra,re);
        PUT32(rb,rf);
        dowait();
        PUT32(rb,rc);
        PUT32(ra,rd);
        PUT32(rb,re);
        PUT32(rb,rf);
        dowait();
    }
}
