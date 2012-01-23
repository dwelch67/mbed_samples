
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm

LLCOPS = -march=thumb

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts

stuff = -O2 -std-compile-opts -strip-debug



all : xmega01.gcc.thumb.bin xmega01.clang.thumb.norm.bin xmega01.clang.thumb.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

xmega01.gcc.thumb.o : xmega01.c blinker.h
	$(ARMGNU)-gcc $(COPS) -mthumb -c xmega01.c -o xmega01.gcc.thumb.o

xmega01.gcc.thumb.bin : memmap vectors.o xmega01.gcc.thumb.o
	$(ARMGNU)-ld -o xmega01.gcc.thumb.elf -T memmap vectors.o xmega01.gcc.thumb.o
	$(ARMGNU)-objdump -D xmega01.gcc.thumb.elf > xmega01.gcc.thumb.list
	$(ARMGNU)-objcopy xmega01.gcc.thumb.elf xmega01.gcc.thumb.bin -O binary


xmega01.clang.bc : xmega01.c blinker.h
	clang $(LOPS) -c xmega01.c -o xmega01.clang.bc

xmega01.clang.thumb.norm.bin : memmap vectors.o xmega01.clang.bc
	llc $(LLCOPS) xmega01.clang.bc -o xmega01.clang.thumb.norm.s
	$(ARMGNU)-as xmega01.clang.thumb.norm.s -o xmega01.clang.thumb.norm.o
	$(ARMGNU)-ld -o xmega01.clang.thumb.norm.elf -T memmap vectors.o xmega01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D xmega01.clang.thumb.norm.elf > xmega01.clang.thumb.norm.list
	$(ARMGNU)-objcopy xmega01.clang.thumb.norm.elf xmega01.clang.thumb.norm.bin -O binary

xmega01.clang.thumb.opt.bin : memmap vectors.o xmega01.clang.bc
	opt $(OOPS) xmega01.clang.bc -o xmega01.clang.thumb.opt.bc
	llc $(LLCOPS) xmega01.clang.thumb.opt.bc -o xmega01.clang.thumb.opt.s
	$(ARMGNU)-as xmega01.clang.thumb.opt.s -o xmega01.clang.thumb.opt.o
	$(ARMGNU)-ld -o xmega01.clang.thumb.opt.elf -T memmap vectors.o xmega01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D xmega01.clang.thumb.opt.elf > xmega01.clang.thumb.opt.list
	$(ARMGNU)-objcopy xmega01.clang.thumb.opt.elf xmega01.clang.thumb.opt.bin -O binary

dumphex : dumphex.c
	gcc -o dumphex dumphex.c

blinker.h : blinker01.s.hex dumphex
	./dumphex blinker01.s.hex


blinker01.s.hex : blinker01.s
	avra -fI blinker01.s


clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f blinker01.s.*


