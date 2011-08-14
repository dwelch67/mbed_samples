
extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET8 ( unsigned int );

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

#define STCTRL   0xE000E010
#define STRELOAD 0xE000E014
#define STCURR   0xE000E018

#define FIO1DIR2 0x2009C022
#define FIO1SET2 0x2009C03A;
#define FIO1CLR2 0x2009C03E;

#define SCS       0x400FC1A0
#define CLKSRCSEL 0x400FC10C

#define PLL0FEED    0x400FC08C
#define PLL0CON     0x400FC080
#define PLL0CFG     0x400FC084
#define PLL0STAT    0x400FC088

#define CCLKCFG     0x400FC104

#define T0CR        0x40004004
#define T0TC        0x40004008
#define T0PR        0x4000400C



void pll_init(void)
{
    //enable the main oscillator
    PUT32(SCS,1<<5);
    while((GET32(SCS)&(1<<6))==0) continue;

//1. Disconnect PLL0 with one feed sequence if PLL0 is already connected.
    //powers up disconnected
    //PUT32(PLL0CON,GET32(PLL0CON)&(~2));
    //PUT32(PLL0FEED,0xAA);
    //PUT32(PLL0FEED,0x55);
    //while((GET32(PLL0STAT) & (1<<25)) != 0) continue;
//2. Disable PLL0 with one feed sequence.
    //powers up disabled
    //PUT32(PLL0CON,GET32(PLL0CON)&(~1));
    //PUT32(PLL0FEED,0xAA);
    //PUT32(PLL0FEED,0x55);
    //while((GET32(PLL0STAT) & (1<<24)) != 0) continue;
//3. Change the CPU Clock Divider setting to speed up operation without PLL0, if desired.
    //PUT32(CCLKCFG,0);
//4. Write to the Clock Source Selection Control register to change the clock source if
//needed.
    PUT32(CLKSRCSEL,1);
//5. Write to the PLL0CFG and make it effective with one feed sequence. The PLL0CFG
//can only be updated when PLL0 is disabled.
    //An N of 1 and M of 12 gives a pllclk of 288MHz. remember the register uses M-1 and N-1
    //otherwise you get a clock slower than you were planning.
    PUT32(PLL0CFG,(12-1));
    PUT32(PLL0FEED,0xAA);
    PUT32(PLL0FEED,0x55);
//6. Enable PLL0 with one feed sequence.
    PUT32(PLL0CON,1);
    PUT32(PLL0FEED,0xAA);
    PUT32(PLL0FEED,0x55);
//6.1 not listed in the manual as something to do
    while((GET32(PLL0STAT) & (1<<24)) == 0) continue;
//7. Change the CPU Clock Divider setting for the operation with PLL0. It is critical to do
//this before connecting PLL0.
    //288/3 = 96 MHZ
    PUT32(CCLKCFG,(3-1));
//8. Wait for PLL0 to achieve lock by monitoring the PLOCK0 bit in the PLL0STAT register,
    while((GET32(PLL0STAT) & (1<<26)) == 0) continue;
//9. Connect PLL0 with one feed sequence.
    PUT32(PLL0CON,3);
    PUT32(PLL0FEED,0xAA);
    PUT32(PLL0FEED,0x55);
//9.1 not listed in instructions
    while((GET32(PLL0STAT) & (1<<25)) == 0) continue;
}


void notmain ( void )
{
    unsigned int ra,rb,rc,rd,re,rf;

    unsigned int lastcount,nowcount;

    pll_init();


    //init GPIO
    ra=GET8(FIO1DIR2);
    ra|=0xB4;
    PUT8(FIO1DIR2,ra);

    ra=FIO1SET2;
    rb=FIO1CLR2;
    rc=0x80;
    rd=0x20;
    re=0x10;
    rf=0x04;

    PUT8(rb,rc);
    PUT8(rb,rd);
    PUT8(rb,re);
    PUT8(rb,rf);


    PUT32(T0PR,(120-1));
    PUT32(T0CR,1);  //enable timer
    lastcount=GET32(T0TC);
    while(1)
    {
        PUT8(ra,rc);
        while(1)
        {
            nowcount=GET32(T0TC);
            nowcount-=lastcount; //upcounter
            if(nowcount>=25000000) break;
        }
        lastcount+=25000000;
        PUT8(rb,rc);
        while(1)
        {
            nowcount=GET32(T0TC);
            nowcount-=lastcount; //upcounter
            if(nowcount>=25000000) break;
        }
        lastcount+=25000000;
    }
}

