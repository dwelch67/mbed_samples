
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm

LLCOPS = -march=thumb

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts

stuff = -O2 -std-compile-opts -strip-debug



all : xmega02.gcc.thumb.bin xmega02.clang.thumb.norm.bin xmega02.clang.thumb.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

xmega02.gcc.thumb.o : xmega02.c blinker.h
	$(ARMGNU)-gcc $(COPS) -mthumb -c xmega02.c -o xmega02.gcc.thumb.o

xmega02.gcc.thumb.bin : memmap vectors.o xmega02.gcc.thumb.o
	$(ARMGNU)-ld -o xmega02.gcc.thumb.elf -T memmap vectors.o xmega02.gcc.thumb.o
	$(ARMGNU)-objdump -D xmega02.gcc.thumb.elf > xmega02.gcc.thumb.list
	$(ARMGNU)-objcopy xmega02.gcc.thumb.elf xmega02.gcc.thumb.bin -O binary


xmega02.clang.bc : xmega02.c blinker.h
	clang $(LOPS) -c xmega02.c -o xmega02.clang.bc

xmega02.clang.thumb.norm.bin : memmap vectors.o xmega02.clang.bc
	llc $(LLCOPS) xmega02.clang.bc -o xmega02.clang.thumb.norm.s
	$(ARMGNU)-as xmega02.clang.thumb.norm.s -o xmega02.clang.thumb.norm.o
	$(ARMGNU)-ld -o xmega02.clang.thumb.norm.elf -T memmap vectors.o xmega02.clang.thumb.norm.o
	$(ARMGNU)-objdump -D xmega02.clang.thumb.norm.elf > xmega02.clang.thumb.norm.list
	$(ARMGNU)-objcopy xmega02.clang.thumb.norm.elf xmega02.clang.thumb.norm.bin -O binary

xmega02.clang.thumb.opt.bin : memmap vectors.o xmega02.clang.bc
	opt $(OOPS) xmega02.clang.bc -o xmega02.clang.thumb.opt.bc
	llc $(LLCOPS) xmega02.clang.thumb.opt.bc -o xmega02.clang.thumb.opt.s
	$(ARMGNU)-as xmega02.clang.thumb.opt.s -o xmega02.clang.thumb.opt.o
	$(ARMGNU)-ld -o xmega02.clang.thumb.opt.elf -T memmap vectors.o xmega02.clang.thumb.opt.o
	$(ARMGNU)-objdump -D xmega02.clang.thumb.opt.elf > xmega02.clang.thumb.opt.list
	$(ARMGNU)-objcopy xmega02.clang.thumb.opt.elf xmega02.clang.thumb.opt.bin -O binary

dumphex : dumphex.c
	gcc -o dumphex dumphex.c

blinker.h : blinker01.s.hex dumphex
	./dumphex blinker01.s.hex


blinker01.s.hex : blinker01.s
	avra -fI blinker01.s



blinker02.s.hex : blinker02.s
	avra -fI blinker02.s

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f blinker01.s.*
	rm -f blinker02.s.*
	rm -f dumphex


