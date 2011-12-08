
//-------------------------------------------------------------------
//-------------------------------------------------------------------
void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET8 ( unsigned int );
//-------------------------------------------------------------------

#define STCTRL   0xE000E010
#define STRELOAD 0xE000E014
#define STCURR   0xE000E018

#define FIO1DIR2 0x2009C022
#define FIO1SET2 0x2009C03A
#define FIO1CLR2 0x2009C03E

#define SCS       0x400FC1A0
#define CLKSRCSEL 0x400FC10C

//-------------------------------------------------------------------
#include "common.h"
//-------------------------------------------------------------------
//-------------------------------------------------------------------
void clock_init ( void )
{
    PUT32(SCS,1<<5);
    while((GET32(SCS)&(1<<6))==0) continue;

    PUT32(CLKSRCSEL,1);
}
//------------------------------------------------------------------------
void change_led_state ( unsigned int led, unsigned int state )
{
    unsigned int ra,rb;

    if(state) ra=FIO1CLR2; else ra=FIO1SET2;
    switch(led&3)
    {
        case 0: rb=0x80; break;
        case 1: rb=0x20; break;
        case 2: rb=0x10; break;
        case 3: rb=0x04; break;
    }
    PUT8(ra,rb);
}
//------------------------------------------------------------------------
unsigned int next_prand ( unsigned int x )
{
    if(x&1)
    {
        x=x>>1;
        x=x^0xBF9EC099;
    }
    else
    {
        x=x>>1;
    }
    return(x);
}
//-------------------------------------------------------------------
unsigned int get_timer_tick ( void )
{
    //down counter, make it look like an up counter?
    //24 bit counter...
    return((0-GET32(STCURR))&0x00FFFFFF);
}
//-------------------------------------------------------------------
unsigned int uart_tx_if_ready ( unsigned int x )
{
    return(1);
}
//-------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    clock_init();

    //init GPIO
    ra=GET8(FIO1DIR2);
    ra|=0xB4;
    PUT8(FIO1DIR2,ra);
    //init cortex-m3 clock
    PUT32(STCTRL,0x00000004);
    PUT32(STRELOAD,0xFFFFFFFF);
    PUT32(STCTRL,0x00000005);

    PUT8(FIO1CLR2,0xB4);

    threada(INIT);
    threadb(INIT);
    threadc(INIT);
    threadd(INIT);
    while(1)
    {
        threada(HEARTBEAT);
        threadb(HEARTBEAT);
        threadc(HEARTBEAT);
        threadd(HEARTBEAT);
    }

    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
