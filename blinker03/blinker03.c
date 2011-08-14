
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

void dowait ( void )
{
    unsigned int ra;
    ra=1;

    while(ra)
    {
        if(GET32(STCTRL)&0x00010000) ra--;
    }
}


void notmain ( void )
{
    unsigned int ra,rb,rc,rd,re,rf;

    //init GPIO
    ra=GET8(FIO1DIR2);
    ra|=0xB4;
    PUT8(FIO1DIR2,ra);


    PUT32(STCTRL,0x00000004); //disabled, no ints, use cpu clock
    PUT32(STRELOAD,0xFFFFFFF);
    PUT32(STCTRL,0x00000005); //enabled, no ints, use cpu clock

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
