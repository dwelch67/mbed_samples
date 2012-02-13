
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm

LLCOPS = -march=thumb

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts

stuff = -O2 -std-compile-opts -strip-debug




all : ired01.gcc.bin ired01.clang.norm.bin ired01.clang.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

ired01.gcc.o : ired01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c ired01.c -o ired01.gcc.o

ired01.gcc.bin : memmap vectors.o ired01.gcc.o
	$(ARMGNU)-ld -o ired01.gcc.elf -T memmap vectors.o ired01.gcc.o
	$(ARMGNU)-objdump -D ired01.gcc.elf > ired01.gcc.list
	$(ARMGNU)-objcopy ired01.gcc.elf ired01.gcc.bin -O binary


ired01.clang.bc : ired01.c
	clang $(LOPS) -c ired01.c -o ired01.clang.bc

ired01.clang.norm.bin : memmap vectors.o ired01.clang.bc
	llc $(LLCOPS) ired01.clang.bc -o ired01.clang.norm.s
	$(ARMGNU)-as ired01.clang.norm.s -o ired01.clang.norm.o
	$(ARMGNU)-ld -o ired01.clang.norm.elf -T memmap vectors.o ired01.clang.norm.o
	$(ARMGNU)-objdump -D ired01.clang.norm.elf > ired01.clang.norm.list
	$(ARMGNU)-objcopy ired01.clang.norm.elf ired01.clang.norm.bin -O binary

ired01.clang.opt.bin : memmap vectors.o ired01.clang.bc
	opt $(OOPS) ired01.clang.bc -o ired01.clang.opt.bc
	llc $(LLCOPS) ired01.clang.opt.bc -o ired01.clang.opt.s
	$(ARMGNU)-as ired01.clang.opt.s -o ired01.clang.opt.o
	$(ARMGNU)-ld -o ired01.clang.opt.elf -T memmap vectors.o ired01.clang.opt.o
	$(ARMGNU)-objdump -D ired01.clang.opt.elf > ired01.clang.opt.list
	$(ARMGNU)-objcopy ired01.clang.opt.elf ired01.clang.opt.bin -O binary




clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s

