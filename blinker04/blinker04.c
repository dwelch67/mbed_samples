
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

void dowait ( void )
{
    unsigned int ra;
    ra=5;

    while(ra)
    {
        if(GET32(STCTRL)&0x00010000) ra--;
    }
}


void notmain ( void )
{
    unsigned int ra,rb,rc,rd,re,rf;

    PUT32(SCS,1<<5);
    while((GET32(SCS)&(1<<6))==0) continue;

    PUT32(CLKSRCSEL,1);

    //init GPIO
    ra=GET8(FIO1DIR2);
    ra|=0xB4;
    PUT8(FIO1DIR2,ra);

    PUT32(STCTRL,0x00000004);
    PUT32(STRELOAD,12000000-1);
    PUT32(STCTRL,0x00000005);

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

    while(1)
    {
        PUT8(ra,rc);
        dowait();
        PUT8(rb,rc);
        dowait();
    }
}

