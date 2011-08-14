
See top level README file to find the mbed schematic and LPC users
manual used heavily in these sample programs.


Looking at the schematic UART0 TX and RX are the signals TGT_SBL_TXD
and TGT_SBL_RXD.  They are tied to the usb interface chip and are
no doubt the serial port called SerialPC by the mbed.org docs.

Looking at the uart chapter there is a list of things you need to do
the good thing is that UART0 has a lot of those things done already.

Power in the PCONP register, uart0 is powered on by default.
PCLKSEL0 shows that the PCLK_UART0 is CCLK/4.

Baud rate, will get to that.

The fifo and basically all of the uart0 config registers, will get to
that.

From the schematic we know P0.2 is TXD0 and P0.3 is RXD0, these are
managed by PINSEL0 which is a 0x00000000 after reset.  bits 5:4 are
GPIO with a setting if 0b00, we want a setting of 0b01 connecting TXD0
(from UART0) to that pin.  Likewise we want 0b01 on bits 7:6 to
select RXD0

Definitely not interested in interrupts nor dma, just want to poll
status bits and send or receive characters.

Back to baud rate.  First off if we dont mess with clocks we come up
with the on chip 4MHz setting for CCLK, the power up setting in PCLKSEL0
is CCLK/4 so our PCLK for UART0 comes up as 1MHz.

If we follow the flow chart for baud rate calculation at the time
of this writing section 14.4.12.1 in the LPC users manual.

Since I know the answer, at least with my mbed, we want 9600 baud.  So
following the flow chart

1000000 / (16 * 9600) = 6.5... It is not an integer, false

start with FRest of 1.5

DLest is int(1000000 / (16 * 9600 * 1.5) = 4
FRest is 1000000 / (16 * 9600 * 4) = 1.6276
That is between 1.1 and 1.9 so we are done

DLM = 0
DLL = 4
DIV = 5
MUL = 8

1000000 / ( 16 * 9600 * (1 + (DIV/MUL))) = 9615 baud.

There are web based calculators and such but I wrote my own, see
uartdiv.c, set pclk to your pclk and dbaud to desired baud, recompile
and run (use a normal host compiler gcc uartdiv.c -o uartdiv, this is
not a cross compile and run on ARM program).

    pclk=1000000;
    dbaud=9600;

It gives a lot of results ending in

...
dl 0x04 mul 0x03 div 0x02 baud 9375 diff 225
dl 0x04 mul 0x05 div 0x03 baud 9766 diff 166
dl 0x04 mul 0x08 div 0x05 baud 9615 diff 15
dl 0x05 mul 0x0A div 0x03 baud 9615 diff 15
dl 0x06 mul 0x0C div 0x01 baud 9615 diff 15

In theory it follows all the rules like div must be less than mul and
hmm, dl (dlm/dll) maybe has to be greater than 2, that may be a rule.
Anyway, we see the dl divisor of 4, mul 8, div 5 combination in there
giving us 9615 as well as two others that give 9615 and nothing
that gets closer than 15 to 9600.  So run with that.

This uart is basically an intel 8250 like, which is not the best uart
to clone, but everyone does, oh well.  (the fractional divisor stuff
is not intel 8250 like, the whole number divisor is).  You can follow
the uart_init code to set up 9600 baud No parity 8 bits 1 stop bit.

The mbed website talks about finding and using the virtual uart on
linux it is /dev/ttyACMx since it is the only ACM based one on this
computer I see it as /dev/ttyACM0 and use minicom to look at it, that
is beyond the scope of this sample.  had to set it for 9600 baud to
see stuff.  Setting the lpc to usb chip to faster than 9600 baud
didnt seem to change that, so made it match.  Inside and beyond the
usb interface chip on the mbed the baud rate may be totally bogus and
virtual, so going faster on the microcontroller may actually make
it faster overall.  will worry about that some other day.

the example program outputs 12345678 then goes into an echo loop,
if you type a character on your serial terminal you should see it
echo back.  pressing the big middle button on the mbed will re-load/reset
the program and you should see the 12345678 again.  If you are having
trouble on the host side put the mbed into an infinite loop sending
something like 0x55 (the capital letter U).
