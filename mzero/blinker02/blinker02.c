
#define GPIO_BASE 0x50000000

#define GPIO_DIR1 (GPIO_BASE+0x2004)
#define GPIO_SET1 (GPIO_BASE+0x2204)
#define GPIO_CLR1 (GPIO_BASE+0x2284)

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

void dowait ( void )
{
    unsigned int ra;

    for(ra=0x20000;ra;ra--)
    {
        dummy(ra);
    }
}

void notmain ( void )
{
    unsigned int ra,rb,rc,rd,re,rf;

    ra=GET32(GPIO_DIR1);
    ra|=1<<8;
    ra|=1<<9;
    ra|=1<<10;
    ra|=1<<11;
    PUT32(GPIO_DIR1,ra);

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
