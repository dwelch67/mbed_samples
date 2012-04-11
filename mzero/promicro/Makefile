
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm -ccc-host-triple $(ARMGNU)

LLCOPS = -march=thumb -mtriple=$(ARMGNU)

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts

stuff = -O2 -std-compile-opts -strip-debug



all : promicro.gcc.thumb.bin promicro.clang.thumb.norm.bin promicro.clang.thumb.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

promicro.gcc.thumb.o : promicro.c blinker.h
	$(ARMGNU)-gcc $(COPS) -mthumb -c promicro.c -o promicro.gcc.thumb.o

promicro.gcc.thumb.bin : memmap vectors.o promicro.gcc.thumb.o
	$(ARMGNU)-ld -o promicro.gcc.thumb.elf -T memmap vectors.o promicro.gcc.thumb.o
	$(ARMGNU)-objdump -D promicro.gcc.thumb.elf > promicro.gcc.thumb.list
	$(ARMGNU)-objcopy promicro.gcc.thumb.elf promicro.gcc.thumb.bin -O binary


promicro.clang.bc : promicro.c blinker.h
	clang $(LOPS) -c promicro.c -o promicro.clang.bc

promicro.clang.thumb.norm.bin : memmap vectors.o promicro.clang.bc
	llc $(LLCOPS) promicro.clang.bc -o promicro.clang.thumb.norm.s
	$(ARMGNU)-as promicro.clang.thumb.norm.s -o promicro.clang.thumb.norm.o
	$(ARMGNU)-ld -o promicro.clang.thumb.norm.elf -T memmap vectors.o promicro.clang.thumb.norm.o
	$(ARMGNU)-objdump -D promicro.clang.thumb.norm.elf > promicro.clang.thumb.norm.list
	$(ARMGNU)-objcopy promicro.clang.thumb.norm.elf promicro.clang.thumb.norm.bin -O binary

promicro.clang.thumb.opt.bin : memmap vectors.o promicro.clang.bc
	opt $(OOPS) promicro.clang.bc -o promicro.clang.thumb.opt.bc
	llc $(LLCOPS) promicro.clang.thumb.opt.bc -o promicro.clang.thumb.opt.s
	$(ARMGNU)-as promicro.clang.thumb.opt.s -o promicro.clang.thumb.opt.o
	$(ARMGNU)-ld -o promicro.clang.thumb.opt.elf -T memmap vectors.o promicro.clang.thumb.opt.o
	$(ARMGNU)-objdump -D promicro.clang.thumb.opt.elf > promicro.clang.thumb.opt.list
	$(ARMGNU)-objcopy promicro.clang.thumb.opt.elf promicro.clang.thumb.opt.bin -O binary


dumphex : dumphex.c
	gcc -o dumphex dumphex.c

blinker.h : blinker02.s.hex dumphex
	./dumphex blinker02.s.hex


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


