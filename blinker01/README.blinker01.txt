
You will need both the schematic and users guide for this one
(see the top level README file).

This sample starts the foundation for the tools for generic ARM
embedded development.  There are a number of good, commercial, ARM
development products out there, in particular the products from ARM
itself.  The focus here will be on open source or otherwise freely
available tools.

These tools are all availble both on windows and linux in particular
(and other *nixes), but these samples were developed on Linux.

http://llvm.org has a well designed set of tools which may have
originally had other design goals, the side effect is a nice multiple
target compiler.  When you have binutils or gcc binaries they are
targeted at a specific processor/family.  With llvm the bulk of it
is processor independent.  it is possible to only build the backend for
certain targets but the default is to build for all supported targets
so a single install set of llvm binaries likeley supports compling for
several processor families. ARM being one of them with thumb and
thumb2 support.  It is true, for example if you do this:

  clang myprog.c -o myprog

Only one processor/target is supported with the llvm linker.  The way
I am using it here is to use it as a cross compiler to assembly, which
is where all of the targets are supported.

The gnu tools, gcc and binutils are also demonstrated, the same source
code (where possible) will use both gcc and clang (llvm).  The assembler
and linker from binutils are used across the board.

Although it is simple to build a gcc cross compiler and target specific
binutils (instructions will be provided eventually) http://www.codesourcery.com
provides a free Lite version of their gnu based tools.   I dont have
anything but good things to say about Codesourcery providing gnu
based ARM tools.  And if you want pay for tool support, they are in
the business of providing that.  Since these examples do not use C
libraries and try to avoid gcclib libraries, either the linux gnueabi or
the non-linux eabi version (change the ARMGNU environment variable in
the Makefiles to switch).  At Codesourcerys website, at the moment the
product is named codebench (it has changed names over time, and perhaps
will again).  Go to codebench, editions, lite, select arm and then select
download the current release.  Ideally for this type of work you want
the EABI edition.  Also note if you are on an ubuntu 64 bit host
computer, these (and perhaps other) binaries you download are 32 bit.
You need to
    sudo apt-get install ia32-libs
so that your 64 bit linux can execute 32 bit binaries.  Otherwise, even
though the binaries are in the path it will say cannot find such and such
gcc file, it will drive you crazy, you can specify the entire filename
and it will say file not found.

Llvm also provides source code and you can build those tools yourself.
Binaries are available, and for now it is easier to just get a pre-built
toolchain (the llvm tools take a LONG time to build).  If running
ubuntu you can simply
  sudo apt-get install llvm

Enough of that let's get to the sample program.

ARM has recently renamed their manuals.  Used to be you would look for
the ARM ARM (ARM Architectural Reference Manual).  And for the core
specific stuff you would look for the core specific TRM (Technical
Reference Manual).

Now what it appears they have is a family specific ARM ARM.

Start at http://infocenter.arm.com

Along the left side under Contents 5 or 6 down is ARM architecture,
click on that.  And click on Reference Manuals to expand that.

The cortex-m3 is the name of the core, but the architecture version
name is ARMv7-M as you can see on the right side of the page when
the Reference Manuals link is selected on the left.

So expand the ARMv7-M Architectural Reference Manual section by clicking
on it.  Now on the right side you can download the manual.  You need
this manual, so if you have to register to get it you have to register
(Registration is FREE).

Teaching ARM/thumb assembly for example is beyond the scope of this
project.  But I will give a general introduction to this manual.
In particular so that you get a feel for differences bewteen the
cortex-M and the more traditional ARM architecture.  The instruction
set is documented in this manual, the cortex-m3 only supports thumb
and thumb2 instructions, ARM instructions are not supported.  Note the
difference between the various encodings ones like T1 that say All
versions of the Thumb instruction set are the original 16 bit thumb
instructions.  The ones that say ARMv7-M are part of the thumb2
instruction set.  Thumb instructions will work on other arm platforms
back to the ARM7 (ARMv4T), Thumb2 is a new 32 bit, variable instruction
length thing for cores like this.  My thumbulator instruction set
simulator, also at github, is strictly the original thumb instruction
set and thumb2 instructions are beyond the scope.  Programs that work
on thumbulator can be transferred almost directly to this chip for
example, but not necessarily true in the other direction.  In addition
to gcc and llvm examples, this project will show thumb vs thumb2
(where practical).

What we need from the ARMv7-M ARM.  If you are an old time
ARM programmer you are used to a 32 bit, ARM (32 bit instruction),
exception table that includes instructions used to branch to the
handler.  The cortex-m3 does NOT do that.  This core is very much
like the other folks (non-ARM processors), it is a list of ADDRESSES,
NOT instructions.  And unlike the other folks the first entry is
the desired powerup setting for the stack pointer.

What is as of this writing the System Level Programmers' Model chapter
ARMv7-M exception model section.

Exception number definition: A list of exception numbers are listed,
1 Reset, 2 NMI, 3 HardFault, etc.

The vector table: word (ARM defines a word as 32 bits) offset in table
0 SP_main, then the rest are exception number based (exception number
times 4 bytes).

So that is the first thing we have in the source blinker01.s

.word   0x10008000  /* stack top address */
.word   _start      /* 1 Reset */
.word   hang        /* 2 NMI */
.word   hang        /* 3 HardFault */
.word   hang        /* 4 MemManage */

Yep, that means we do not have to initialize the stack pointer manually
after reset.  By the time you start executing code it is set to whatever
you defined.

Now we switch to the LPC1768 users manual.  One of the first chapters
is the memory map.  This is supposed to be a 64KByte part, we see
that there is up to 32KB of on chip sram 0x10000000 to 0x10007FFF
and then it says AHB SRAM 16KBytes at 0x2007C000 - 0x2007FFFF
and 16KBytes at 0x20080000 - 0x20083FFF for devices with 64KBytes
of total SRAM.  So it sounds like the 64KBytes advertised is total
memory, but broken into parts.  So as you see above the stack pointer
will be set to the top of the 32KBytes 0x10008000 (stack pointer is
decremented before used so 0x10008000 means the first stack item is
at address 0x10007FFC).

In order to blink the leds we next need to look at the schematic

There are four leds along the short edge of the card.  On the third
page of the schematic labelled LED-1, LED-2, LED-3, and LED-4.
We see that those are connected to pins 32, 34, 35, 37.  Which are
ports  P1.18, P1.20, P1.21, and P1.23.  That is the info we need to
take back to the users manual.

Note/remember that ARM makes cores, not chips, when it comes time to
learn about ARM stuff will come from ARM website documents.  The GPIO
interface, registers, etc are all NXP (a.k.a. LPC, Philips, etc).  Point
being what you learn here about the GPIO interface is specific to this
chip and probably similar to other LPC/NXP chips, but the same ARM
cortex-m3 core in some other vendors chip (ti, stm, etc) are going to
have different programmers interfaces.  You need to go through this
exercise every time you change chips.

As of this writing the GPIO chapter in the users guide is
Chapter 9: LPC17xx General Purpose Input/Output (GPIO).

Right up front it says  1) Power: always enabled, some vendors require
you to enable (power) the gpio block, feed it clocks and do various
things.  Following a link in this section to what is as of this writing
8.3 Pin function select register, shows pin function select. 00
is the default setting and that says primary function, typicially
GPIO port.   We need to drive down into this and find PINSEL3 (0x4002C00C)
where P1.18, P1.20, P1.21 and P1.23 are defined.  All four say the
reset value is 0b00 and the function when 0b00 is GPIO Port.  So to use
these pins as GPIO we do not need to do anything (assuming the
users guide is correct).

Back to the GPIO chapter.

There appears to be a myriad of different ways to get at the same GPIO
pins.  We are going to need to do three things.
1) make sure these GPIO lines are set to output so that we can
drive the leds.
2) know how to set the output to a one for each line
3) know how to set the output to a zero for each line

Looking at the register description section the FIODIR, FIOSET
and FIOCLR registers do what we want.  Now we could stop right
here and use 0x2009C020, 0x2009C038, and 0x2009C03C as 32 bit
registers, but we are going to dig deeper.

If you look at FIO1DIR2 it describes this as controlling the directon
for the 8 GPIO lines P1.16 to P1.23 with P1.16 being bit 7.
Likewise FIO1SET2 and FIO1CLR2 are 8 bit registers for the same ports.

This gives us these register addresses

FIO1DIR2 0x2009C022
FIO1SET2 0x2009C03A
FIO1CLR2 0x2009C03E

Back to the schematic breifly.  We are using 4 I/O pins these registers
affect 8 I/O pins.  What else is affected by these 8 port registers?

P1.16 and P1.17 go to the ethernet phy, looks like MDIO stuff.
P1.19 and P1.22 are not connected.  The reset value for FIO1DIR2 is
0x00 and looking at the definition a 0 means the pin is an input.

Some microcontrollers/processors you dont have this SET and CLR scheme
sometimes you have a single OUT register that the state of the
register is how it tries to drive the output pin, and we would definitely
need to be careful with a register that has more than just our leds
on it.  But because this chip has these SET and CLR registers that
are defined as not doing anything if you write a zero you can
change the state of a single I/O pin without affecting the others,
without having to do a read-modify-write.

It would be good programming practice to not make assumptions about
the reset state of these configuration registers.  A compilete driver
would include configuring all the bits related to the interface.  These
examples are not necessarily going to use complete drivers.  Another
argument here would be that this is a very resource limited platform
and ideally you would deploy a part like this in a system configured
one way only, as a result generic, proper, drivers consume valuable
resources.

After the I/O lines are configured as outputs, the code goes into an
infinite loop that turns one led on and the other three off.  Then
delays using a counter loop (using timers will come in future samples)
so that we can see the led blink with our eyes.  This sample blinks
the leds in a Knight Rider style for those old enough to remember that
TV show.

Examine the Makefile to see the commands needed to build.

Since this example is a single assembler file, llvm/clang is not
used.

The gnu assembler (binutils) is used to assemble the source file to
an object.  The gnu linker is used to take the source file and turn
it into a binary, in this case .elf.

The gnu linker is massively configurable and the docs are hard to
follow, here is a linker script that works with the current gnu linker:

/* memmap */
MEMORY
{
   rom(RX)   : ORIGIN = 0x00000000, LENGTH = 0x40000
   ram(WAIL) : ORIGIN = 0x10000000, LENGTH = 30K
}

SECTIONS
{
   .text : { *(.text*) } > rom
   .bss  : { *(.bss*) } > ram
}

the names rom and ram are arbitrary, you could use bob and ted here
it doesnt matter.  Using ram and rom makes it a bit more obvious what
they are.  From the memory map in the users manual you see where
these numbers come from (left a couple of KBytes for the stack).

The (RX) and (WAIL) are very important, without those letters in
parenthesis you might have to have a lot more stuff in the linker script
R and X are Read only and eXecute, basically read-only data like
const something something data in a C program. And execute is the program
machine code itself.  The other letters WAIL are like read/Write data
and some other things.  Basically if you are building a program that
is all in one ram then use all the letters.

MEMORY
{
   ram(RXWAIL) : ORIGIN = 0x10000000, LENGTH = 30K
}

If you have a situation where you want your program (.text) in rom and
read/write data (.bss, .data, etc) in ram then split out the R and X

MEMORY
{
   rom(RX)   : ORIGIN = 0x00000000, LENGTH = 0x40000
   ram(WAIL) : ORIGIN = 0x10000000, LENGTH = 30K
}

The second half

SECTIONS
{
   .text : { *(.text*) } > rom
   .bss  : { *(.bs*) } > ram
}

helps get the .text (machine code, read only data, etc) stuff to go
to the memory section we want.  The .bss line appears to be needed
to get that zero init data into ram.

This is about as minimal of a linker script as you can get, and
the SECTIONS as defined here does not necessarily work for older
versions of binutils, and I would not be surprised if some day in the
future it stops working and I will have to change again.  I didnt not
create this from scratch I borrowed and tweaked this knowledge from
other projects similar to this one.

For as many different developers you find examples from, you will find
as many different linker scripts.  Crafting linker scripts is harder
IMO than any of this low level peripheral programming.

By separating the compiler, assembler, and linker we can avoid the
inclusion of C libraries and the link step looking for ctr0.o to boot
the processor/program.

Is important to know that the gnu linker links objects into the binary
in the order presented on the command line, so it is a good idea
to have the boot code you want at the beginning of the binary be the
first object on the command line.  With a single file this is not
an issue but when we start to mix C with some assembler to boot the
processor, it will matter.

Once you have the tools and build the binary there will be a .list file
created.  Looking at the beginning of this file:

Disassembly of section .text:

00000000 <hang-0x50>:
   0:   10008000    andne   r8, r0, r0
   4:   0000005b    andeq   r0, r0, fp, asr r0
   8:   00000050    andeq   r0, r0, r0, asr r0
   c:   00000050    andeq   r0, r0, r0, asr r0

...

0000005a <_start>:
  5a:   4819        ldr r0, [pc, #100]  ; (c0 <mainloop+0x50>)
  5c:   7801        ldrb    r1, [r0, #0]
  5e:   22b4        movs    r2, #180    ; 0xb4

I HIGHLY recommend you disassemble your elf files (BEFORE you add
commands to create .bin files), this dump is where you find out
if your linker script is working, and you verify what you think you
put in .text is in .text and what you think is .bss or .data is in fact
.bss or .data.  You may quickly realize a table you wanted/needed
in rom is in ram.

Note that when in thumb mode on an ARM processor the lsbit of the
program counter is 1.  For processors that support both ARM and thumb
instructions the way you branch between ARM code and thumb code
is through bit 0 of the address.  So exception number 1, reset
is address 0x5B, but the actual address in ram is 0x5A.  The lsbit
being on is the more correct setting for this processor.  Being a
thumb/thumb2 only processor (does not run ARM instructions) I dont
know what happens if you dont have that bit set correctly.  As a general
ARM family of chips programmer though you should strive to get this
right.

How you get this right when writing assembler is by using .thumb_func

Putting the .thumb_func before the definition of _start

.thumb_func
.globl _start
_start:

tells the assembler that the _start label is a thumb address, meaning
set bit 0 of the address.  If you comment out or remove that .thumb_func
and re-build the binary you will see that the address changes to 0x5A
in the vector table, which is not correct.

The .thumb near the beginning of the file (same as .code 16) tells the
assembler that what follows is to be assembled as thumb code.  Why
the assembler does not know that labels in this section are thumb
destinations is a mystery.

Looking at our .list file we see that the whole program is in the
.text section, as desired.  So it is safe to use objcopy to extract
out just the binary (rom/ram image) from the .elf file.  If we had
both .text and .data in that .elf file then we would not safely be
able to do that, say we had a single 1 byte .data item it would have
been at address 0x10000000, objcopy would have created a 0x10000001
byte .bin file 256MBytes.  Other platforms say you had ram start
at 0x80000000 that would be a 2 Gigabyte file.  A pretty big mistake,
so dont use objcopy to create .bin files until you have verified that
you have not made any mistakes, or that you need to do more work to
create a .bin file that will go into a flash/rom.

The beauty of the mbed.org platform is that when you plug a usb cable
from your computer into it it mounts as a mini usb-flash-drive,
basically a mounted file system.  It expects you to copy a .bin file
to this virtual flash drive.  The Makefile shows you how to use objcopy
to copy the actual binary data (rom/ram image) from the elf file into
a .bin file.  Notice that I prefer to name my cross compiled elf files
as .elf so as not to be confused with elf files compiled for the host
system.  Also to help immediately distinguish the various binary formats
elf, coff, intel hex, srec, binary, etc.  I dont know what happens if
you have more than one .bin file on the mbed, I re-use the same file
name on the mbed.  Once copied, press the big button in the middle
and it will load and boot your new program.  Much much much simpler
for a new embedded/microcontroller developer to get up and running than
other approaches.  It also appears that the .bin file is programmed into
the rom not simply loaded in ram.  Looking at the list file if the
exception table was not needed the program would have executed through
that data and slammed into the hang: b hang loop.  Add to that when
you remove and replace power to the board it boots up with the last
program you loaded.

Very long winded but that should help remove the confusion and fear
of trying to use the mbed outside the web based sandbox.  Demonstrate
how easy it is to use this board with or without the web based sandbox.
