
/*

The cortex-m3 has a very nice and simple timer built in.  24bit, runs
off the cpu clock or an external clock.  Ideally you use it to generate
a specific system interrupt at some interval.  But no reason you have
to use it that way, clearly designed for polling as well.

If you have followed the blinker01 and blinker02 examples there are
hand tuned timing loops to slow things down so the human eye can see
the leds change.  The system clearly comes up and lets us just jump
right in and change GPIO pins, etc.  But what is the clock rate
of the cpu?  Eventually we will want to use the UART, but to drive
the uart baud rate at some reasonable/standard rate we need to know
what clock the chip is running off of so we can divide it down properly.
Also, microcontrollers like this give lots of clock control, for example
an on board clock, the external clock, which on this board is 12MHz, and
some onboard clock multipliers which are supposed to, in theory, give
us up to 100MHz on this chip.  Why not run that all the time or by
default?  Burns too much power, microcontrollers are often put in low
or battery powered devices, so it is better to have it boot off the
slowest clock and have software make it go faster if desired.  Which
is eventually what we desire.

So we need to both experimentally and through document research determine
what the clock rate is that we are running.

There is a link on the mbed.org site under Implementation Details about
the NXP LPC1768 hardware, currently this is the link:

http://mbed.org/handbook/mbed-NXP-LPC1768

On that page you will see a link to the schematic and a link to a
users manual and datasheet.

We need both the schematic and the users manual to understand
this program.  From the prior blinker programs we know what GPIO
pins are tied to the four leds, and where in the users manual to
program those GPIO registers.

This exercise involves looking up things in the users manual, currently
chapter 23: LPC17xx System Tick Timer and Chapter 4: LPC17xx Clocking
and power control.  Always expect chapter numbers or names to change
as vendors improve their products or documentation.

The register description of the system tick timer is very simple, when
enabled the 24-bit down counter loads the value in the register
STRELOAD, it counts down to zero.  When it gets to zero it sets bit
16 in the STCTRL register, if enabled that causes a system tick
interrupt.  Then it re-loads the STRELOAD counter and starts counting
down again.

We want to poll.  The COUNTFLAG bit in the STCTRL register states
that it is clear on read.  For starters let's set the recount register
to its max value 0xFFFFFF (it is a 24 bit counter), and between changing
the state of the led (on, off, on, off...) let's start by waiting for
one downcount period.  If the led blinks too fast then we will need
to wait for multiple down count periods before changing the state
of the led.

If you build and run this you will see that the led changes state
every 4 seconds or so.

If you do the math 0xFFFFFF / 4 seconds is a little over 4MHz, definitely
not the 12MHz on the schematic, so we are either using the 12MHz divided
by 3 or we are not using that oscillator.


We have tied this system timer to the cpu clock, so let's go look
at the clock chapter to see what is up.

This chapter starts off with text like this:

The LPC17xx includes three independent oscillators. These are the Main
Oscillator, the Internal RC Oscillator, and the RTC oscillator. Each
oscillator can be used for more than one purpose as required in a
particular application. This can be seen in Figure 7. Following Reset,
the LPC17xx will operate from the Internal RC Oscillator until switched
by software. This allows systems to operate without any external
crystal, and allows the boot loader code to operate at a known
frequency.

That makes a lot of sense.  And in the description of the Internal RC
oscillator it states that the nominal frequency is 4MHz.  And that
solves our mystery.  We powered up on the 4MHz internal oscillator.

Good to know.

*/

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


    //init system timer for polling.
    //is read-modify-write really necessary?
if(0)
{
    ra=GET32(STCTRL);
    ra&=(~3); //disable, no interrupts
    ra|=0x4;  //use cpu clock
}
else
{
    ra=0x00000004; //disabled, no ints, use cpu clock
}
    PUT32(STCTRL,ra);
    PUT32(STRELOAD,0xFFFFFFF);
    ra|=1;    //enable timer
    PUT32(STCTRL,ra);

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

