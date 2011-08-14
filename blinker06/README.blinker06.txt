
Where to find the schematic and users manual is described in the
top level README file.

You definitely need to read the blinker05 README first, you should be
reading all of these in order anyway.

With blinker05 we succeeded in setting the processor to run at 100Mhz
but we had to divide the 12MHz external clock to do it.  96Mhz
can be reached without dividing that clock up front.

M = (pllclk * N) / (2 * fin);

We want an N of one and a minimal M, the first M that puts the pllclk
at a valid rate is 12 which gives 288MHz.  Dividing that by 3 gives
96Mhz to the processor.

If we use the same numbers from blinker05 which counted to 120 seconds
but we run it at 96MHz instead of 100MHz, then it should take

120 * 100 / 96 = 125 seconds.  And that is what we get, so we are
now running 96MHz.

