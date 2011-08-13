
blinker01 and blinker02 README files go to great lengths to explain
the boot process and other details related to these files and what
is going on in the Makefile.  This sample does not need to go into
that it can discuss just the meat of the program.


The prior samples use a counter loop approach to delaying the code so
that the LED blink rate is slow enough to see.  Although it works
as you see with blinker02 the blink rate from the same C code varies
dramatically.  In the good old days the assembly language manuals
would give you detailed information about number of clock cycles used
for each instruction, from that you would/could hand count and hand
craft timing.  You dont see much of that anymore in general.  You might
still see a lot of it in the PIC world though, and other places where
the clock speed of the processor is similar to the rates the signals
you are sampling or generating.  With the mbed we can get up to 100MHz
on the processor, if you are trying to bit bang i2c signals and want
to program in C (the reason why the clock rate is so high) you will
want to use the on board timers.

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
an on chip generated clock, the external clock, which on this board is
12MHz, and some on chip clock multipliers which are supposed to, in
theory, give us up to 100MHz on this chip.  Why not run that fast clock
all the  time or by default?  Burns too much power for one thing,
microcontrollers are often put in low or battery powered devices, so
it is better to have it boot off the slowest clock and have software
make it go faster if desired.  Which is eventually what we desire, but
that is another sample program.

So we need to both experimentally and through document research
determine what the clock rate is that we are running.

From the prior blinker programs we know what GPIO pins are tied to the
four leds, and where in the users manual to program those GPIO
registers.

At the time of this writing the users manual (see the top level README
for where to find this document) has chapters named
Chapter 23: LPC17xx System Tick Timer
and
Chapter 4: LPC17xx Clocking and power control
Always expect chapter numbers or names to change as vendors improve
their products or documentation.

The register description of the system tick timer is very simple, when
enabled the 24-bit down counter loads the value in the register
STRELOAD, it counts down to zero.  When it gets to zero it sets bit
16 in the STCTRL register, if enabled that causes a system tick
interrupt.  Then it re-loads the STRELOAD counter and starts counting
down again.

We want to poll.  The COUNTFLAG bit in the STCTRL register states
that it is cleared on a read.  And we can disable the interrupt.
Since we dont know what speed the processor is running (yet) let's
start by setting the recount register to the max value 0xFFFFFF (the
docs say this is a 24 bit down counter).  And between changing the state
of the led (on, off, on, off...) let's start by waiting for one
downcount period.  If the led blinks too fast then we will need
to wait for multiple down count periods before changing the state
of the led.

If you build and run this you will see that the led changes state
every 4 seconds or so.  (also notice we are already not dependent on
the compiler.  the same results with llvm unoptimized as gcc).

If you do the math 0xFFFFFF / 4 seconds is a little over 4MHz, definitely
not the 12MHz on the schematic, so we are either using the 12MHz divided
by 3 or we are not using that oscillator.


We have tied this system timer to the cpu clock in the STCTRL register,
so let's go look at the clock chapter to see what is up.

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
