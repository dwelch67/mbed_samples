
This sample adds to the blinker01 sample (see the blinker01 readme
in order to be able to understand this sample).  Where to get the
llvm and gcc tools is described in the blinker01 README.

This sample demonstrates booting the processor using a little bit
of assembler but the bulk of the program being written in C.

vectors.s, just like blinker01.s, boots the processor.  It very
quickly though calls a function named notmain.  Why not main()?  Well
I have seen in the past that when you have a main() the linker wants
to drag in other libraries.  Using a non-standard name emphasises
this is an embedded program, assembler with some code written in C for
simplicity as opposed to a desktop C application running on an
operating system making C and system api calls, etc.  Also, I have
seen every compiler I had used mess up when generating memory (I/O)
accesses using the wrong instruction.  A 32 bit register that gets
optimized into an 8 bit access.  Yep, volatile and all the keywords you
have learned would not fix it.  In addition much of my work involves
testing this code against simulated logic or writing my own simulated
logic in order to get the software in place before or when the hardware
arrives.  So both for getting the correct access type and for reasons
to abstract the hardware the C code uses PUT8(), GET8(), PUT32(),
GET32(), etc.  You take a cost in performance (you are using C with
gcc or llvm you are already not interested in performance) but get
paid back ten fold in flexibility, debugging, etc.  These memory/register
access functions are implemented in assembler.  Normally I use
a separate file like putget.s but since this is a small, simple example
and vectors.s is already there they have been implemented in vectors.s.
(yes I know that ldrb [r0],r0 is not supported on some ARM cores).

blinker02.c contains the bulk of the sample code.  Very similar to
blinker01.s, we turn on one led at a time (turning off the other three
for no particular reason) and use a simple counter based delay loop
to slow the on/off times so that we can see them with our slow human
eyes.  Volatile and not-optimizing could/would avoid this but by
using an external function (not found until link time) in the delay
loop we force the compiler to generate the counter loop.  It is a quick
and easy way to prevent the optimizer from removing code you want
present to intentially slow things down.

Because this program is in C and it uses a counter based delay loop
it is expected to be a little to a lot slower than the assembly only
blinker01 sample.

Being a C program now we can get into gcc and llvm and thumb and thumb2
code generation.  Those four combinations plus two more are demonstrated.
Because this C program is so simple you may find that no thumb2
instructions are used, and the thumb/thumb2 binaries are the same.

With gcc your input is C (or C++) and the ouptput is a target specific
binary.  With llvm, the way we are using it here, clang takes the C
code and turns it into a binary form of the llvm internal runtime language,
bytecode for lack of a better term.  Then, llvm provides tools for
working with these bytecode files, for example you can compile all of
your individual .c files, then join them into a single bytecode file
and perform a project wide optimization, before converting from the
bytecode format finally to the target specific assembler then binutils
is used to take it the last mile.  Up to the point that llc is used
the steps are not target specific, its not ARM code until llc, you would
use the same steps if targeting a powerpc or msp430 or whatever.  gcc
has nothing remotely close to resembling this flexibility and
functionality.

I have removed the llvm/clang build example, this is now gnu only.  I
am still a huge fan of llvm and clang, but their command lines are
a moving target and I am not going to go through and touch every example
every rev or worse try to deal with supporting mulitple revs.  Likewise
I supoort thumb2 extensions here.  The remaining will just build for
generic thumb, can always refer to this Makefile if you feel you need
thumb2 extensions.

Using -nostdlib -nostartfiles -ffreestanding on the gcc command line as
a general habit tells gnu not to try to include C/C++ libraries, I
will provide my own _start: entry point, etc.  Basically undo all the
fancy stuff and go back to making this just a C to asm or in this
case assembled asm compiler.  These items man not make sense when
compile to object -c is used, I always have them there just in case
I have to let gcc do the assembling and linking.  (same as ld, the
order of the items on the command line affects who is where in the
binary (unless the linker script calls objects out specifically)).
We do not need to do any of this with clang, there will be times where
clang specific command line options will be needed when the compiler
tries to include llvm specific library calls that dont match the
similar gcc library calls, for example.  But for these samples we
dont have to add much to clang.

This code does not use any of the standard include files, no include
files for that matter.  Because no C or operating specific libraries
are required/desired.  This is very intentional.  Another thing you dont
see me do is this:

unsigned int bob = 7;
...
void myfun ( void )
{
   ...
   something = ra + bob;
   ...
}

Instead my (embedded) code does this:

unsigned int bob;
...
void myfun ( void )
{
   bob = 7;
   ...
   something = ra + bob;
   ...
}

There is a noticeable difference.  In one case the constant 7 is in
the .data segment in the other case the 7 is in the instructions in
the .text segment.  If you look at the blinker01 README you will
understand the concept. If you have a .data segment (separate from
a .bss segment) then you have to have boot code that copies the .data
segment from rom to the right location in ram.  This is more work,
more stuff to maintain, etc.  Likewise you have to get that .data
segment into the rom in the first place.  There are probably linker
script ways to do this but you start to get very toolchain dependent.
It is a very good programming habit to initialize your variables before
you use them and I am happy to see that gcc is starting to give
warnings about this.  Basically never assume that the variable
initializes to zero:

unsigned int bob;
...
void myfun ( void )
{
   ...
   something = ra + bob++;
   ...
}

Always initialize it to something IN YOUR CODE before you use it.  This
helps you in desktop applications as well as embedded.

You will also never see me do this:

void myfun ( void )
{
   unsigned int bob = 7;
   ...
   something = ra + bob;
   ...
}

Instead:

void myfun ( void )
{
   unsigned int bob;
   ...
   bob = 7;
   ...
   something = ra + bob;
   ...
}

Why?  How many times have you seen this:

void myfun ( void )
{
   static unsigned int bob = 7;
   ...
   something = ra + bob;
   ...
}

That makes it a global variable (available only to this function)
and puts it in the .data segment and we are back to that discussion.
Not doing it makes the static something that is not haphazardly added.

Yes, all of this "I only do it this way" stuff has to do with this
sample program.  If you are here and have never booted a processor
at this low of a level, you may take my sample program and instaly fail
as soon as you do one of the above things.  If you want to do the
above, you are more than welcome to but you need to read the blinker01
readme and then do your own research into how to get the contents
of the .data segment into rom and then write a routine between _start:
and before any calls to C code that copies the .data data from rom
to ram.

What is not demonstrated here is that once you start using global variables
(or local globals as I like to call them, static local variables) you
either get .data or .bss.  Other compilers use other names, but the
gnu world uses .bss for the zero init memory for global variables.
your boot code after reset and before the first C function is called
needs to zero this memory out.  Not shown here as I mentioned but it is
a very good idea to have your boot code zero or at least initialize all
of ram, eventually you will use a system that has parity or ecc protected
memory and you can get into trouble in a hurry depending on the alignment
of the parity/ecc and the size of things your stack for example uses.
(64 bit memory with 32 bit stack items for example, instant ecc fault
on the entry to the first C function).

Also not demonstrated here, but today with these read disturb problems
we are seeing with these newer technology flash memories, it is (generally)
a bad idea to execute from flash (these flash based microcontrollers
of course are the exception).  A simple way to handle this is to build
your entire embedded application as if you were going to load it into
ram and run it.  You can in that case very simply have a .bss and .data
and all that in one linear memory segment, when you make a .bin your
.bss and .data are in the .bin.  You then take a very small bit of
boot code that boots up in the flash, the application .bin file is
also in the flash, and you copy it to ram then branch to it.  You dont
have to get into fancy, toolchain specific, linker scripts and all that
your programs just work.

Enough with the general purpose embedded programming stuff, lets get
back to the specifics of this sample.

I am having binutils do the assembling and linking for both compilers.
I am letting gcc go straight to object file but clang to bytecode
to assembler to object eventually.  Although arguably more reliable
not using an optimizer resultsin in considerably larger and considerably
slower code.  Not good for embedded.  Balance the risk of opitimzation
with the reward, and learn how to manipulate and manage the optimizer
(comes with experience).  With gcc I let gcc do the optimization.
With clang I specifically do not want clang doing any optimization.
llvm has a program called opt that does bytecode level optimization,
I prefer to use that program.  It doesnt apply to this program but
when I have progects with lots of C files I prefer to use llvm-link
to link them into one bytecode file.  The opt can optimize across
those files.  Gcc is only going to optimize within the single C files
it is given, there is no coperable project wide optimizer.  Also through
experiments and through common sense the more meat you give the optimizer
to chew on the better it will do.  So if clang is allowed to optimize
then opt has less to work with, so I dont let clang optimize.  Now
when you go from bytecode to assembler with llc, llc has a target specific
optimizer.  Unlike most tools though the default is -O2, normally with
C compilers for example if you dont specify then you get no optimization
(-O0).  That is why llc doesnt have a command line optimization specified
it is already at the desired level.

Thumb/thumb2.  As discussed in the blinker01 readme, there is a difference
between thumb and thumb2.  Thumb2 is an extension using undefined
instructions in thumb to create a variable word length instruction set
to bridge the performance gap between ARM programs and thumb programs
(apples to apples thumb is normally slower, fewer registers, a smaller
simpler instruction set takes more instructions to do the same task,
the exception being platforms like the Gameboy Advance with 16 bit,
non-zero wait state memory, thumb is naturally faster there as
processor clocks are secondary to memory cycles).  So thumb2 tries
to balance the small binary size of thumb and performance of ARM.

With gcc if you specify -mthumb -mcpu=cortex-m3 -march=armv7-m it will
generate thumb/thumb2 code from the C code it is compiling.  If you
specify -mthumb without the cpu and arch then it will only generate
thumb and not generate thumb2.  The makefile for this project creates
both a thumb and thumb2 based binary using gcc as the C compiler. Using
the 2011.03 version from codesourcery both binaries use thumb instructions
and are basically the same small size.  Using the 2010.09 version though
the thumb2 version is a much larger binary, and does use thumb2
instructions.

There are so many different paths to a binary with llvm.  The path
shown here is to have clang turn the C code into bytecode, just like
the .elf extension I like to put .bc so I know what the file is.  To
demonstrate using and not using the C optimizer (but leaving in the
target specific llc optimizer) there are twice as many llvm based
binaries built.  .norm. is without the C optimizer and .opt. is with.
(neither gcc nor clang is it really a C optimizer it is internal code
icode or bytecode being optimized).  And thumb vs thumb2.  Llvm
release29 is being used at the time of this writing.  The llvm based
binaries are a little larger and slower than gnu.  it happens sometimes
llvm makes smaller faster binaries than gnu.   So the path through
llvm shown here is clang gets you from C to bytecode.  If the
optimizer is used it goes from bytecode to bytecode with a different
name.  Then llc is used to get from bytecode to target specific
assembly language files.  Then gnu bintuils are used to assemble and
link.  If I had more than one .c file I would have used llvm-link
to link many .bc files to one .bc file then opt to optimize the one
.bc file and llc to make one .s file.  When llc is given -mthumb
by itself it generates thumb instructions.  When you give it
-mthumb -mcpu=cortex-m3 it can/will use thumb2 instructions.

When you invoke gcc what happens is another binary program is called
that takes the C code, turns it into internal code, similar but different
to the internal bytecode used by llvm.  All the optimization stuff
you asked for, etc are done, and target specific assembly code is
written to a .s file (yeah, really).  And then the binutils gnu
assembler (gas) is called to turn that into object code.  If you have
not specified -C on the command line then gnu linker is called to
turn the object into a binary file (elf).  Then some of the intermediate
files like the assembly language .s file are deleted.   Although clang
can and will take a .c file in and give you an elf file out if you let it
by using -emit-llvm we are telling it to remain in that intermediate
code space, and then we can use the other tools directly to manipulate
that code space.  The steps in the process are not really different
between the tools, what you have direct access to and what you dont
are different.  Remember that llvm's original design goal is to make
generic virtual machine binaries and just in time (JIT) compile to the
target to execute.  So you pretty much have to break the generic vm
code tools out from the last step to the target.  With the gnu tools
they have no such goals, it is just a compiler toolchain so the guts
are hidden from view.

Another very long winded README file.  The desire was to cover embedded
C tool stuff (for ARM) and not simply describe a few dozen lines of C
code (that was already described in assembly language form in the
prior sample).

