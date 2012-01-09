

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern unsigned int PUTGETCLR ( unsigned int, unsigned int );
extern unsigned int PUTGETSET ( unsigned int, unsigned int );

extern void dummy ( unsigned int );

#define GPIO_BASE 0x50000000

#define GPIO_W00 (GPIO_BASE+0x1000)
#define GPIO_W01 (GPIO_BASE+0x1004)
#define GPIO_W02 (GPIO_BASE+0x1008)
#define GPIO_W03 (GPIO_BASE+0x100C)
#define GPIO_W04 (GPIO_BASE+0x1010)
#define GPIO_W05 (GPIO_BASE+0x1014)
#define GPIO_W06 (GPIO_BASE+0x1018)
#define GPIO_W07 (GPIO_BASE+0x101C)


#define GPIO_DIR0 (GPIO_BASE+0x2000)
#define GPIO_PIN0 (GPIO_BASE+0x2100)
#define GPIO_SET0 (GPIO_BASE+0x2200)
#define GPIO_CLR0 (GPIO_BASE+0x2280)

#define GPIO_DIR1 (GPIO_BASE+0x2004)
#define GPIO_SET1 (GPIO_BASE+0x2204)
#define GPIO_CLR1 (GPIO_BASE+0x2284)

#define STCTRL   0xE000E010
#define STRELOAD 0xE000E014
#define STCURR   0xE000E018

#define PDRUNCFG        0x40048238
#define SYSOSCCTRL      0x40048020
#define SYSPLLCLKSEL    0x40048040
#define SYSPLLCLKUEN    0x40048044
#define MAINCLKSEL      0x40048070
#define MAINCLKUEN      0x40048074
#define SYSAHBCLKCTRL   0x40048080
#define SYSAHBCLKDIV    0x40048078
#define SYSPLLCTRL      0x40048008
#define SYSPLLSTAT      0x4004800C


#define PIO0_7          0x4004401C

int clock_init ( void )
{
    unsigned int ra;

    if(1)
    {
        //A little safety measure.  At the time of this writing it is
        //easy to hang the ARM if you make a mistake configuring the
        //clocks such that the mbed interface chip cannot re-program it
        //(for example to remove the code in flash you used to hang it)
        //One solution is to put a jumper to ground on pin 5 of the
        //microcontroller, causing the microcontroller to boot in ISP
        //mode, basically from a different program in flash which does
        //not hang the arm.  This pin is hard to get to on the mbed but
        //GPIO 0.7 is easy to get to.  If there is a jumper between the
        //two corners, ground and PIN21 then this function will abort
        //and not configure the clock.  This gives you a little more
        //freedom to mess with the clock init code and not have the
        //recovery as painful.

        //if GPIO 0.7 is shorted to ground, then bypass clock config
        PUTGETSET(SYSAHBCLKCTRL,(1<<16)); //need this for I/O in general
        PUTGETSET(SYSAHBCLKCTRL,(1<<6)); //specifically make sure gpio is enabled
        PUT32(PIO0_7,0x00000010); //use it as a GPIO with a pull up resistor
        PUTGETCLR(GPIO_DIR0,1<<7); //make it an input
        for(ra=0;ra<200;ra++) dummy(0); //just in case it needs to settle
        if(GET32(GPIO_W07)==0xFFFFFFFF)
        {
            //keep going
        }
        else
        {
            //do not re-configure the clocks
            return(1);
        }
    }

    PUTGETCLR(PDRUNCFG,1<<5); //power up system oscillator
    PUT32(SYSOSCCTRL,0x00000000);
    for(ra=0;ra<200;ra++) dummy(0); //kill some time for it to come up

    PUT32(SYSPLLCLKSEL,0x00000001); //select crystal oscillator as pll input
    PUT32(SYSPLLCLKUEN,0x00000001);
    PUT32(SYSPLLCLKUEN,0x00000000);
    PUT32(SYSPLLCLKUEN,0x00000001);
    while((GET32(SYSPLLCLKUEN)&0x00000001)==0) continue; //wait for it to become the pll input
    //if you are using the pll
    if(1)
    {
        //M = 4 (3+1), P = 2 (1*2)  P is for the PLL not a divisor of the
        //main clock.  So this results in 12 * 4 / SYSAHBCLKDIV
        PUT32(SYSPLLCTRL,0x00000023);
        PUTGETCLR(PDRUNCFG,1<<7); //power up system pll
        while((GET32(SYSPLLSTAT)&0x00000001)==0) continue; //wait for PLL to lock
    }

    PUT32(MAINCLKSEL,0x00000003); //select main clock source, pll output
    PUT32(MAINCLKUEN,0x00000001);
    PUT32(MAINCLKUEN,0x00000000);
    PUT32(MAINCLKUEN,0x00000001);
    while((GET32(MAINCLKUEN)&0x00000001)==0) continue; //wait for it

    PUT32(SYSAHBCLKDIV,0x00000001); //divide by 1 gives us 48MHz

    //not using usb
    PUTGETSET(PDRUNCFG,(1<<10));
    PUTGETSET(PDRUNCFG,(1<<8));

    PUTGETSET(SYSAHBCLKCTRL,(1<<16)); //need this for I/O in general
    PUTGETSET(SYSAHBCLKCTRL,(1<<6)); //specifically make sure gpio is enabled
    return(0);
}

void dowait ( void )
{
    unsigned int ra;
    ra=16;

    while(ra)
    {
        if(GET32(STCTRL)&0x00010000) ra--;
    }
}

void notmain ( void )
{
    unsigned int ra,rb,rc,rd,re,rf;

    clock_init();

    ra=GET32(GPIO_DIR1);
    ra|=1<<8;
    ra|=1<<9;
    ra|=1<<10;
    ra|=1<<11;
    PUT32(GPIO_DIR1,ra);

    PUT32(STCTRL,0x00000004); //disabled, no ints, use cpu clock
    PUT32(STRELOAD,12000000-1); //this is a 24 bit register not 32
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
