
//-------------------------------------------------------------------
//-------------------------------------------------------------------

extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET8 ( unsigned int );

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

//-------------------------------------------------------------------

#define PINSEL0     0x4002C000

#define U0ACR       0x4000C020
#define U0LCR       0x4000C00C
#define U0DLL       0x4000C000
#define U0DLM       0x4000C004
#define U0IER       0x4000C004
#define U0FDR       0x4000C028
#define U0FCR       0x4000C008
#define U0TER       0x4000C030

#define U0LSR       0x4000C014
#define U0THR       0x4000C000
#define U0RBR       0x4000C000

//-------------------------------------------------------------------
void uart_init ( void )
{
    unsigned int ra;

    ra=GET32(PINSEL0);
    ra&=(~(0xF<<4));
    ra|=0x5<<4;
    PUT32(PINSEL0,ra);
//1000000 Hz PCLK 9600 baud
//dl 0x04 mul 0x08 div 0x05 baud 9615 diff 15
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
void notmain ( void )
{
    unsigned int ra;

    uart_init();
    hexstring(0x12345678,1);
    while(1)
    {
        ra=uart_getc();
        uart_putc(ra);
        if(ra==0x0D) uart_putc(0x0A);
    }
}
//-------------------------------------------------------------------
