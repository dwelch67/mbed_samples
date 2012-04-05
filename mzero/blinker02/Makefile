
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm -ccc-host-triple $(ARMGNU)

LLCOPS = -march=thumb -mtriple=$(ARMGNU)

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts

stuff = -O2 -std-compile-opts -strip-debug




all : blinker02.gcc.thumb.bin blinker02.clang.thumb.norm.bin blinker02.clang.thumb.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

blinker02.gcc.thumb.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker02.c -o blinker02.gcc.thumb.o

blinker02.gcc.thumb.bin : memmap vectors.o blinker02.gcc.thumb.o
	$(ARMGNU)-ld -o blinker02.gcc.thumb.elf -T memmap vectors.o blinker02.gcc.thumb.o
	$(ARMGNU)-objdump -D blinker02.gcc.thumb.elf > blinker02.gcc.thumb.list
	$(ARMGNU)-objcopy blinker02.gcc.thumb.elf blinker02.gcc.thumb.bin -O binary


blinker02.clang.bc : blinker02.c
	clang $(LOPS) -c blinker02.c -o blinker02.clang.bc

blinker02.clang.thumb.norm.bin : memmap vectors.o blinker02.clang.bc
	llc $(LLCOPS) blinker02.clang.bc -o blinker02.clang.thumb.norm.s
	$(ARMGNU)-as blinker02.clang.thumb.norm.s -o blinker02.clang.thumb.norm.o
	$(ARMGNU)-ld -o blinker02.clang.thumb.norm.elf -T memmap vectors.o blinker02.clang.thumb.norm.o
	$(ARMGNU)-objdump -D blinker02.clang.thumb.norm.elf > blinker02.clang.thumb.norm.list
	$(ARMGNU)-objcopy blinker02.clang.thumb.norm.elf blinker02.clang.thumb.norm.bin -O binary

blinker02.clang.thumb.opt.bin : memmap vectors.o blinker02.clang.bc
	opt $(OOPS) blinker02.clang.bc -o blinker02.clang.thumb.opt.bc
	llc $(LLCOPS) blinker02.clang.thumb.opt.bc -o blinker02.clang.thumb.opt.s
	$(ARMGNU)-as blinker02.clang.thumb.opt.s -o blinker02.clang.thumb.opt.o
	$(ARMGNU)-ld -o blinker02.clang.thumb.opt.elf -T memmap vectors.o blinker02.clang.thumb.opt.o
	$(ARMGNU)-objdump -D blinker02.clang.thumb.opt.elf > blinker02.clang.thumb.opt.list
	$(ARMGNU)-objcopy blinker02.clang.thumb.opt.elf blinker02.clang.thumb.opt.bin -O binary




clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s

