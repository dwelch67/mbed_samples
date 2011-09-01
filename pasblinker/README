
Well here is a fun one.  The freepascal.org folks are working on an
embedded pascal compiler.  The lpc1768 is supported so we can start
using it.


You will need a binutils with the right arm-embedded- prefix.

> wget http://ftp.gnu.org/gnu/binutils/binutils-2.21.1a.tar.bz2

If you dont have wget then just use the link in a web browser

You might will need these things in order to build binutils

> sudo apt-get install build-essential libmpfr-dev libgmp3-dev libmpc-dev

Extract, build, and install binutils

> tar xjf binutils-2.21.1a.tar.bz2
> cd binutils-2.21.1
> ./configure --target=arm-linux --prefix=/opt/embarm/ --program-prefix=arm-embedded- --disable-werror
> make
> sudo make install

Add it to your path when you want to use it (will need it both to build
fpc embedded and when using fpc embedded).

> PATH=/opt/embarm/bin/:$PATH

You will need fpc to build fpc

> sudo apt-get install fpc

Get the sources to fpc

> svn co http://svn.freepascal.org/svn/fpc/trunk fpc
> cd fpc

And build it.

> make clean buildbase installbase CROSSINSTALL=1 OS_TARGET=embedded CPU_TARGET=arm SUBARCH=armv7m

Copy the pasblinker.pp file here

> cp /path/to/pasblinker.pp .

Compile the pascal program

> ./compiler/ppcrossarm -Parm -Tembedded -Wplpc1768 -Cparmv7m pasblinker.pp
> arm-embedded-objcopy pasblinker.elf -O binary pasblinker.bin

Copy the binary to the mbed board

> cp pasblinker.bin /media/MBED/

And press the button on the mbed board to run/load the binary.

read through the blinker examples to understand what the pasblinker.pp
program is doing
