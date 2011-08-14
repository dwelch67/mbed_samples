
extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET8 ( unsigned int );

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

#define STCTRL   0xE000E010
#define STRELOAD 0xE000E014
#define STCURR   0xE000E018

#define FIO1DIR2 0x2009C022
#define FIO1SET2 0x2009C03A
#define FIO1CLR2 0x2009C03E

#define SCS       0x400FC1A0
#define CLKSRCSEL 0x400FC10C

#define T0CR        0x40004004
#define T0TC        0x40004008
#define T0PR        0x4000400C


//*********************************************************
//  THIS IS AN INTERRUPT HANDLER, DONT MESS AROUND
unsigned int twoled;
void systick_handler ( void )
{
    GET32(STCTRL);
    if(twoled&1)
    {
        PUT8(FIO1CLR2,0x20);
    }
    else
    {
        PUT8(FIO1SET2,0x20);
    }
    twoled++;
}
//*********************************************************


void notmain ( void )
{
    unsigned int ra,rb,rc,rd,re,rf;
    unsigned int lastcount,nowcount;
    unsigned int nticks;
    unsigned int oneled;

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

    oneled=0;
    twoled=0;

    //4MHz, 12000000 counts is 3 seconds
    PUT32(STCTRL,0x00000004);
    PUT32(STRELOAD,12000000-1);
    PUT32(STCTRL,0x00000007); //interrupt enabled

    //4Mhz/4 = 1Mhz  500000 is half a second
    nticks=500000;
    PUT32(T0CR,1);  //enable timer
    lastcount=GET32(T0TC);
    while(1)
    {
        nowcount=GET32(T0TC);
        nowcount-=lastcount; //upcounter
        if(nowcount>=nticks)
        {
            if(oneled&1)
            {
                PUT8(ra,rc);
            }
            else
            {
                PUT8(rb,rc);
            }
            oneled++;
            lastcount+=nticks;
        }
    }

    while(1) continue;
}

