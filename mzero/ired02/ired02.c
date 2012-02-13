

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

extern unsigned int PUTGETCLR ( unsigned int, unsigned int );
extern unsigned int PUTGETSET ( unsigned int, unsigned int );

#define GPIO_BASE 0x50000000

#define GPIO_DIR0 (GPIO_BASE+0x2000)
#define GPIO_PIN0 (GPIO_BASE+0x2100)

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
#define UARTCLKDIV 0x40048098

#define PIO0_17          0x40044044

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
void hexstring ( unsigned int d )
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
    uart_putc(0x0D);
    uart_putc(0x0A);
}
//-------------------------------------------------------------------
void notmain ( void )
{
    unsigned int ra;
    unsigned int a,b,c;
    unsigned int code;


    uart_init();
    hexstring(0x12345678);

    PUTGETSET(SYSAHBCLKCTRL,(1<<16)); //need this for I/O in general
    PUTGETSET(SYSAHBCLKCTRL,(1<<6)); //specifically make sure gpio is enabled
    PUTGETSET(GPIO_DIR1,1<<8);
    PUTGETSET(GPIO_DIR1,1<<9);
    PUTGETSET(GPIO_DIR1,1<<10);
    PUTGETSET(GPIO_DIR1,1<<11);
    PUT32(GPIO_CLR1,(1<<8)|(1<<9)|(1<<10)|(1<<11));

    PUT32(STCTRL,0x00000004); //disabled, no ints, use cpu clock
    PUT32(STRELOAD,0xFFFFFFF);
    PUT32(STCTRL,0x00000005); //enabled, no ints, use cpu clock

    PUT32(PIO0_17,0x00000000);
    PUTGETCLR(GPIO_DIR0,1<<17);


    //12mhz?
    //9ms = 108000
//000188F9 0001913D
//100601 8.38ms  102717 8.55ms
#define STARTMIN 100000
#define STARTMAX 104000
//0000C394 0000C889
//50068 4.17ms  51337 4.28ms
#define CODEMIN 49900
#define CODEMAX 52000
//00001784 00001D79
//6020 7545
#define LOWMIN 5900
#define LOWMAX 8000
//000014C7 00002613
//5319 9747
#define SHORTMIN 5000
#define SHORTMAX 10000
//00004659 00004EAE
//18009 20142
#define LONGMIN 17000
#define LONGMAX 21000



    while(1)
    {
        while(GET32(GPIO_PIN0)&(1<<17)) continue; //to low
        a=GET32(STCURR);
        while(1) if(GET32(GPIO_PIN0)&(1<<17)) break; //to high
        b=GET32(STCURR);
        c=(a-b)&0x00FFFFFF;
        if(c<(STARTMIN)) continue;
        if(c>(STARTMAX)) continue;
        while(GET32(GPIO_PIN0)&(1<<17)) continue; //to low
        a=GET32(STCURR);
        c=(b-a)&0x00FFFFFF;
        if(c<(CODEMIN)) continue;
        if(c>(CODEMAX)) continue;
        while(1) if(GET32(GPIO_PIN0)&(1<<17)) break; //to high
        b=GET32(STCURR);
        c=(a-b)&0x00FFFFFF;
        if(c<(LOWMIN)) continue;
        if(c>(LOWMAX)) continue;
        code=0;
        for(ra=0;ra<32;ra++)
        {
            while(GET32(GPIO_PIN0)&(1<<17)) continue; //to low
            a=GET32(STCURR);
            c=(b-a)&0x00FFFFFF;
            code<<=1;
            if(c<(SHORTMIN)) break;
            if(c>(LONGMAX)) break;
            if(c<(SHORTMAX))
            {
                //code|=0;
            }
            else
            {
                if(c>(LONGMIN))
                {
                    code|=1;
                }
                else
                {
                    break;
                }
            }
            while(1) if(GET32(GPIO_PIN0)&(1<<17)) break; //to high
            b=GET32(STCURR);
            c=(a-b)&0x00FFFFFF;
            if(c<(LOWMIN)) continue;
            if(c>(LOWMAX)) continue;

        }
        if(ra<32) continue;
        hexstring(code);
    }



















}

