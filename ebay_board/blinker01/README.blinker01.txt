
You will need both the schematic and users guide for this one
(see the README file one level up).

Also refer to the blinker01 and other README files for the mbed itself
there is a lot of detail there.

This particular board, not an mbed, nor a clone, what it shares with
the mbed is that it has an LPC1768 microcontroller, thats it.

If you go to ebay and search on this item number 260848402688 (just put
that in the search box), you might get the board.  Otherwise search
for LPC1768.  The board has the square microcontroller chip in the middle
with holes, presuably breakout connections for the microcontroller.
The silkscreen says mcu123.net under the chip and LPC1700/LPC2300 above.
one side has an A sized usb connector the other a full jtag connector.
Three red buttons on three of the corners.

A number of folks are selling what appears to be the same board, and
they are asia based, selling from different countries, so they say.
Some have cheaper or no shipping cost, some have a lot of shipping
(relatively) so shop around and see what you see.

It came with a usb cable, looked to be very cheap, I plugged it into the
board and now it wont come out, you know like when you have too much
stuff in a junk drawer and when you close it and try to open it it
gets stuck on something, except in this case there is no good way to
reach in there.  Or at least I have not tried to slide anything in there
to try to free it.

The various sellers have various web links and such, this one came with
a mini cdrom with some schematics and stuff, you have to wade through
the directories and open files, most directories and filenames are
not in english.

The schematic shows the led is on port 2 pin 0 (2.0).  And despite
the buttons looking like they light up the schematic doesnt show
an led connection there.

I assume this is supposed to have a usb bootloader and show up as a
flash drive just enough to copy a file to it.  Maybe that works on
windows but didnt on linux.  No bother I wanted this one to use jtag
not usb to get into it.

Like the mbed samples this first led blinker is timed using a counting
loop, the number it counts to/from is hand tuned to be slow enough
to see the led blink.  Since I have no clue where these boards really
come from or what is on them the bootloader or crystal on yours may
leave you running a different speed than mine.

Again read the mbed samples they spell out a lot more about what is
going on.  In this case set the direction bit on the port, make sure
the mask is zero (a bit opposite, and confusing, a zero means you can
write a one means you cannot) set or clear the gpio output state to
turn the led on and off.

The ram example runs strictly out of ram no messing with flash.

I am using openocd.  I have a j-link edu, I have a amontek jtag-tiny and
another one.  Each are under $100.

Microcontrollershop.com has a couple of them, good price, and you dont
get killed on the shipping like you do with the amontek and the what was
it turtle something, I dont remember there is another overseas that the
shipping is as much as the product.

http://microcontrollershop.com/product_info.php?cPath=154_171&products_id=3124&osCsid=990de8d571237cd4aeed506f9363a46e

http://microcontrollershop.com/product_info.php?cPath=154_171&products_id=3647&osCsid=990de8d571237cd4aeed506f9363a46e

The j-link edu is lightning fast but for non-commercial use only, no
way would I pay or have the company I work for pay the commercial price
for these things.  Not worth it.  We use amontek jtag-tinys day in and
day out, work great, buy 10 or so and save on shipping.  Or just get
the Signalizer LITE, pretty much the same thing, ftdi based.

google openocd, the sources are not on sourceforge.  download, configure
and install, you must add the appropriate --enable when building you can
enable both the jlink and whatever libftdi thing in a single build and
havea  binary that works on both.

I use three command windows.  One terminal/command window is where I compile
the program, change directory to wherever the sources are.  The second
window also changed the directory to wherever the sources/binaries
are, if you start openocd in the directory with the binaries then you
dont have to specify the path to the binaries, makes life easier.
The third window, once openocd is connected is a telnet session into
openocd for sending commands and getting results.  I guess the fourt
window of course is the text editor with the source for the programs.

In the window that will start openocd, assuming the cfg files from openocd
that I have here work, and depending on the jtag interface you are using:

this:
openocd -f jlink.cfg -f lpc1768.cfg
or this:
openocd -f signalyzer-lite.cfg -f lpc1768.cfg
or this:
openocd -f jtagkey-tiny.cfg -f lpc1768.cfg
or whatever interface you have.

If it finds the microcontroller it will have a line like this:

Info : JTAG tap: lpc1768.cpu tap/device found: 0x4ba00477 (mfg: 0x23b, part: 0xba00, ver: 0x4)
I

Now in another window

telnet localhost 4444

and you will get an openocd prompt

>

Be careful not to type the next command in the wrong window, depending
on permissions you might turn off your computer and have to wait for
it to reboot and start over.

> halt
> load_image blinker01.elf
> resume 0x10000001

halt stops the processor
load_image loads a program from a file, the elf format contains addresses
and data/instructions so the load knows where to put stuff, if you
had only a .bin file then

> load_image blinker01.bin 0x10000000

you have to specify the starting address.  the elf path is simpler and
more reliable (less human error).

Now the ARM instruction set is 32 bit word based, all instructions 32bits
so you dont need the lower two address bits for instruction addresses
(everything is aligned).  Then thumb came out, 16 bit instructions so
address bit 1 is used, and what they did was use address bit 0 to indicate
arm mode vs thumb mode.  If address bit 0 is zero then arm mode and 1
for thumb mode.  This thing needs/is thumb mode only so although the
program was compiled for and loaded into ram at 0x10000000 when you
use resume to start the processor running again you want that lsbit
set, so

> resume 0x10000001

I tend to up arrow three times, halt, up arrow three times, load, up
arrow three times resume as I develop.

If everything went right then you should see the led blinking.

If not you might try using the mdb and mwb commands (memmory dump byte
and memory write byte) to manually mess with the gpio registers and
see if you cant get the led on ir see if you are really connected to
the processor.  type help at the openocd prompt to get a list of
all the commands.



Trashing, I mean writing to rom/flash.  Some docs talk about a bootloader
and that you should base your program at address 0x2000, well I didnt
do that, I based mine at 0x00000000.  See blinker01 in the rom directory.

Same jtag setup as the ram stuff above, in the rom dir though.

Not sure if it took several iterations or what, but where I am now
to write my program to flash, so that when I press reset my flash based
program runs:

OPENOCD TERMINAL PROMPT:

> halt
> flash write_image erase blinker01.elf

Then press reset.






