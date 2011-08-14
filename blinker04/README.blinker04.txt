
This is the same as blinker03 except we want to use the 12MHz main
oscillator instead of the onboard, boot up, 4MHz oscillator.

The LPC microcontrollers can make clock setup quite difficult, even
to read how to do things.

For this particular desired mode, the first thing we figure out is that
PLL0 which is the one that can optionally feed the cpu clock, is
bypassed after reset.  That is good.

Next, to use the main oscillator we need to enable it as an input/clock.
The SCS System Control and Status register, bit OSCEN needs to be set,
then we have to wait until OSCSTAT is set indicating the clock is
stable and we can use it.  Then we can change CLKSRCSEL to a 1 to select
the main clock as a source.

Now we are running at 12MHz instead of 4MHz.  The countdown timer is
set to 12,000,000 - 1 giving 12,000,000 CCLK clocks each cycle.  Change
dowait() to wait for 5 cycles and the led will change state once
every 5 seconds.

BTW, blinking an led like this, and using clock dividers in the chip
to get led changes that are many seconds like 30 or 60 or many minutes
like 5, 10, 15, etc.  You can use a watch (yeah, you know from the
history books what those are) or anything with a minute and second hand
and your eyes to get fairly good precision on hacking your way through
figuring out what clock source something is using.  Also figure out
for example if putting a 4 in a divisor register divides the clock
by 4 or by 5 for example.  You may think you have found all the clock
divisor blocks in some path only to realize you are off by a power of
two or four or something, and trying to count one one thousand, two
one thousand, three one thousand, is not going to be accurate enough
to figure this all out.


