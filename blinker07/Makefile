
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm -ccc-host-triple $(ARMGNU)

LLCOPS = -march=thumb -mtriple=$(ARMGNU)

LLCOPS2 = -march=thumb -mtriple=$(ARMGNU) -mcpu=cortex-m3

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts

stuff = -O2 -std-compile-opts -strip-debug




all : blinker07.gcc.thumb.bin blinker07.gcc.thumb2.bin blinker07.clang.thumb.norm.bin blinker07.clang.thumb.opt.bin blinker07.clang.thumb2.norm.bin blinker07.clang.thumb2.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

blinker07.gcc.thumb.o : blinker07.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker07.c -o blinker07.gcc.thumb.o

blinker07.gcc.thumb2.o : blinker07.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c blinker07.c -o blinker07.gcc.thumb2.o

blinker07.gcc.thumb.bin : memmap vectors.o blinker07.gcc.thumb.o
	$(ARMGNU)-ld -o blinker07.gcc.thumb.elf -T memmap vectors.o blinker07.gcc.thumb.o
	$(ARMGNU)-objdump -D blinker07.gcc.thumb.elf > blinker07.gcc.thumb.list
	$(ARMGNU)-objcopy blinker07.gcc.thumb.elf blinker07.gcc.thumb.bin -O binary

blinker07.gcc.thumb2.bin : memmap vectors.o blinker07.gcc.thumb2.o
	$(ARMGNU)-ld -o blinker07.gcc.thumb2.elf -T memmap vectors.o blinker07.gcc.thumb2.o
	$(ARMGNU)-objdump -D blinker07.gcc.thumb2.elf > blinker07.gcc.thumb2.list
	$(ARMGNU)-objcopy blinker07.gcc.thumb2.elf blinker07.gcc.thumb2.bin -O binary


blinker07.clang.bc : blinker07.c
	clang $(LOPS) -c blinker07.c -o blinker07.clang.bc

blinker07.clang.thumb.norm.bin : memmap vectors.o blinker07.clang.bc
	llc $(LLCOPS) blinker07.clang.bc -o blinker07.clang.thumb.norm.s
	$(ARMGNU)-as blinker07.clang.thumb.norm.s -o blinker07.clang.thumb.norm.o
	$(ARMGNU)-ld -o blinker07.clang.thumb.norm.elf -T memmap vectors.o blinker07.clang.thumb.norm.o
	$(ARMGNU)-objdump -D blinker07.clang.thumb.norm.elf > blinker07.clang.thumb.norm.list
	$(ARMGNU)-objcopy blinker07.clang.thumb.norm.elf blinker07.clang.thumb.norm.bin -O binary

blinker07.clang.thumb.opt.bin : memmap vectors.o blinker07.clang.bc
	opt $(OOPS) blinker07.clang.bc -o blinker07.clang.thumb.opt.bc
	llc $(LLCOPS) blinker07.clang.thumb.opt.bc -o blinker07.clang.thumb.opt.s
	$(ARMGNU)-as blinker07.clang.thumb.opt.s -o blinker07.clang.thumb.opt.o
	$(ARMGNU)-ld -o blinker07.clang.thumb.opt.elf -T memmap vectors.o blinker07.clang.thumb.opt.o
	$(ARMGNU)-objdump -D blinker07.clang.thumb.opt.elf > blinker07.clang.thumb.opt.list
	$(ARMGNU)-objcopy blinker07.clang.thumb.opt.elf blinker07.clang.thumb.opt.bin -O binary

blinker07.clang.thumb2.norm.bin : memmap vectors.o blinker07.clang.bc
	llc $(LLCOPS2) blinker07.clang.bc -o blinker07.clang.thumb2.norm.s
	$(ARMGNU)-as blinker07.clang.thumb2.norm.s -o blinker07.clang.thumb2.norm.o
	$(ARMGNU)-ld -o blinker07.clang.thumb2.norm.elf -T memmap vectors.o blinker07.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D blinker07.clang.thumb2.norm.elf > blinker07.clang.thumb2.norm.list
	$(ARMGNU)-objcopy blinker07.clang.thumb2.norm.elf blinker07.clang.thumb2.norm.bin -O binary

blinker07.clang.thumb2.opt.bin : memmap vectors.o blinker07.clang.bc
	opt $(OOPS) blinker07.clang.bc -o blinker07.clang.thumb2.opt.bc
	llc $(LLCOPS2) blinker07.clang.thumb2.opt.bc -o blinker07.clang.thumb2.opt.s
	$(ARMGNU)-as blinker07.clang.thumb2.opt.s -o blinker07.clang.thumb2.opt.o
	$(ARMGNU)-ld -o blinker07.clang.thumb2.opt.elf -T memmap vectors.o blinker07.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D blinker07.clang.thumb2.opt.elf > blinker07.clang.thumb2.opt.list
	$(ARMGNU)-objcopy blinker07.clang.thumb2.opt.elf blinker07.clang.thumb2.opt.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s

