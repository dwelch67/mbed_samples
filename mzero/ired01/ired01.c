
#define SYSAHBCLKCTRL   0x40048080

#define GPIO_BASE 0x50000000

#define GPIO_DIR0 (GPIO_BASE+0x2000)
#define GPIO_PIN0 (GPIO_BASE+0x2100)

#define GPIO_DIR1 (GPIO_BASE+0x2004)
#define GPIO_SET1 (GPIO_BASE+0x2204)
#define GPIO_CLR1 (GPIO_BASE+0x2284)

#define PIO0_17          0x40044044

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern unsigned int PUTGETCLR ( unsigned int, unsigned int );
extern unsigned int PUTGETSET ( unsigned int, unsigned int );

void notmain ( void )
{
    unsigned int ra;

    PUTGETSET(SYSAHBCLKCTRL,(1<<16)); //need this for I/O in general
    PUTGETSET(SYSAHBCLKCTRL,(1<<6)); //specifically make sure gpio is enabled


    ra=GET32(GPIO_DIR1);
    ra|=1<<8;
    ra|=1<<9;
    ra|=1<<10;
    ra|=1<<11;
    PUT32(GPIO_DIR1,ra);

    PUT32(GPIO_CLR1,(1<<8)|(1<<9)|(1<<10)|(1<<11));

    PUT32(PIO0_17,0x00000000);
    PUTGETCLR(GPIO_DIR0,1<<17);

    while(1)
    {
        if(GET32(GPIO_PIN0)&(1<<17))
        {
            PUT32(GPIO_CLR1,(1<<8));
        }
        else
        {
            PUT32(GPIO_SET1,(1<<8));
        }
    }
}
