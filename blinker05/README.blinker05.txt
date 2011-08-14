
Where to find the schematic and users manual is described in the
top level README file.

Now we are going to use the pll (phase lock loop) to multiply the
external 12MHz oscillator to run the processor as fast as we can go.
This model LPC microcontroller has a 100MHz speed limit on the ARM
core.

Between the external oscillator and the ARM clock we have a divisor
a multiplier and another divisor.  These have to be integers.  And
there are a few more rules.

The pll clock, what you get after the first divide and multiply has
to be between 275 and 550MHz.  And the multiplier must be between 6
and 512.

Ideally you want to not divide a clock, esp this first divisor,
and there is a setting to divide by one and let it pass through so
lets hope for that setting.  This first divisor is the N setting.

M = (pllclk * N) / (2 * fin);

fin is our 12Mhz and N we are going to start with 1.

To keep pllclk between 275 and 550 this means M is between 12 and 22

Using a little math:

12 288.000000 2.880000 96.000000
13 312.000000 3.120000 78.000000
14 336.000000 3.360000 84.000000
15 360.000000 3.600000 90.000000
16 384.000000 3.840000 96.000000
17 408.000000 4.080000 81.600000
18 432.000000 4.320000 86.400000
19 456.000000 4.560000 91.200000
20 480.000000 4.800000 96.000000
21 504.000000 5.040000 84.000000
22 528.000000 5.280000 88.000000

The first number is M, second is pllclk.  Third is pllclk/100 (ideal
number to get 100MHZ).  Since none of these come out as integers the
fourth column is the pllclk divided by the next integer up from the
third column.

This is not surprising really, we have a source clock of 12mhz,
12 has the factors 2*2*3.  100 the factors 2*2*5*5.  We need
to get the 3 out of there.  If we didnt have the pllclk rule we
could probably find something that we could divide by some multiple
of 3.  Say 600mhz would have been an M of 25, we could then divide by
6.  What is interesting is the number 96 keeps coming up, in particular
with the lowest M 288MHz which we are told is what we want for a
cleaner clock a smaller M and smaller N.  (I wouldnt be surprised
if a smaller M means less power consumption)  We will come back to 96MHz.

If we pre-divide the 12MHz by 3 we get essentially 4MHz going into
the pll, this gives a lot more choices:

35 280.000000 2.800000
36 288.000000 2.880000
37 296.000000 2.960000
38 304.000000 3.040000
39 312.000000 3.120000
40 320.000000 3.200000
41 328.000000 3.280000
42 336.000000 3.360000
43 344.000000 3.440000
44 352.000000 3.520000
45 360.000000 3.600000
46 368.000000 3.680000
47 376.000000 3.760000
48 384.000000 3.840000
49 392.000000 3.920000
50 400.000000 4.000000
51 408.000000 4.080000
52 416.000000 4.160000
53 424.000000 4.240000
54 432.000000 4.320000
55 440.000000 4.400000
56 448.000000 4.480000
57 456.000000 4.560000
58 464.000000 4.640000
59 472.000000 4.720000
60 480.000000 4.800000
61 488.000000 4.880000
62 496.000000 4.960000
63 504.000000 5.040000
64 512.000000 5.120000
65 520.000000 5.200000
66 528.000000 5.280000
67 536.000000 5.360000
68 544.000000 5.440000

And note that an M of 50 gives 400MHz on the pllclk with a divide by 4
gives us 100MHz on the processor.

Lets blink the led.

The systick timer is interesting but does not have a lot of resolution
or features.  So we are going to switch to one of the traditional
timers.

Reading up on the timer chapter we find that TIMER 0 and 1 are enabled
after reset by default.  The PCLK reference is CCLK/4 or 25MHz if we
are at 100MHz.

There is a prescaler in the timer, the divisor is the value in the
prescaler + 1.  So after reset it is a divide by 1.  Another way
to say this is to divide by something using the prescaler load
divisor - 1.

There is a 32 bit timer counter register, it simply counts up to
0xFFFFFFFF and rolls over to 0x00000000 and keeps going.

We dont want interrupts or anything else, so other post-reset defaults
are good.

The math gets very nice if we allow the timer counter to roll over
like this.  For demonstration purposes say for example we had an 8 bit
counter 0x00 to 0xFF.  If we wanted to measure say 0x40 counts, we
would take the starting count, when the current count - start count
is greater than or equal to 0x40 we have measured that time.
That is fine if we are counting from say 0x10 to 0x50, but what about
when it rolls over?  Well 0xF0+0x40 is 0x130 right?  Our example
8 bit counter would only be able to have the 0x30 part in it.
If we subtract 0xF0 from 0x30 will we get 0x40?  Well hardware
doesnt subtract it inverts and feeds a carry in of 1 (invert and
add one is how you negate using twos complement right?)
So 0x30 + (~0xF0) + 1 = 0x30 + 0x0F + 1 = 0x30 + 0x10 = 0x40
Works just fine.  So we dont care about the roll over it just works.

As of this writing the timer section of the LCP users manual has some
mistakes.  For example the T0CR register says that when bit 0 is a 1
the timer is enabled, and if 1 the timer is disabled.  It would make
sense that the timer powers up not enabled/running so 0 is probably
disabled and 1 enabled.  You would have to hack through this to
figure it out.

Knowing all of this if you were to leave the prescaler as a divide
by one (T0PR = (1-1) = 0), enabled the counter (T0CR = 1) sample
the T0TC register to get a start count, turn on the led.  Go into
a loop sampling the T0TC register until the sample minus start count
is 25000000 the frequency of our timer clock (PCLK_TIMER0).  And repeat
with the led off.  The led will change once per second if we have
successfully multiplied the clock.

Much care must be taken when polling a timer like this.  If you were
to, for example

change gpio
read start count
wait for current count - start count >= time out
change gpio
read start count
wait for current count - start count >= time out

You are going to incur some error over time, you are making the gpio
changes long, if you are big banging i2c for example you are slowing
down your signals and overall transfer time goes down.

here is why.

read counter and save as count_a
change gpio
read start count
wait for current count - start count >= time out
save current count as count_b
change gpio
read start count
wait for current count - start count >= time out
save current count as count_c

If you were aiming for 30 counts per state change
count_b - count_1 might be 31 for example and count_c - count_b
might be 32 counts, overall count_c - count_a would be 63 counts
and this could/would continue as you are adding the change gpio
code into your time delay.

If instead:

read counter and save as start count
change gpio
wait for current count - start count >= time out
start count += 30
change gpio
read start count
wait for current count - start count = time out
start count += 30

Even if every time you are a little long, say 31 counts, 61 - 31 is 30
91 - 61 is 30, and there is no gain over time, you have the error of the
measurement.

If the software is just too slow, even at 100MHz then you shouldnt be
polling, you probably need to get hardware involved as interrupts are
probably not going to give you any better results.  Assembly and interrupts
might, or might not, depends on how small of a number you are trying
to reach.  Counting clock cycles using hand tuned assembly would be
the most accurate if anything at that point.  You have probably failed
your system engineering, the hardware/software design should have been
done to show this thing you are trying to time is too small for the
solution.

Back to the sample.

So if we use difference in counts of the timer to count to 25 million
and instead of a prescaler of 0 which is a divide by 1, we change
that to a divide by 120 (put a 120-1 to get 120), the led should change
every two minutes.    If we have even a 1% mistake somewhere that will
be over a second wrong, something we should start to be able to see
with our eyes and a watch/stopwatch of some sort.  If there is doubt
change the divisor to a bigger number 5 minutes 10 minutes, amplify the
error.
