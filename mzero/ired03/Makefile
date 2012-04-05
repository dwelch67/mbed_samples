
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm -ccc-host-triple $(ARMGNU)

LLCOPS = -march=thumb -mtriple=$(ARMGNU)

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts

stuff = -O2 -std-compile-opts -strip-debug




all : ired03.gcc.bin ired03.clang.norm.bin ired03.clang.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

ired03.gcc.o : ired03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c ired03.c -o ired03.gcc.o

ired03.gcc.bin : memmap vectors.o ired03.gcc.o
	$(ARMGNU)-ld -o ired03.gcc.elf -T memmap vectors.o ired03.gcc.o
	$(ARMGNU)-objdump -D ired03.gcc.elf > ired03.gcc.list
	$(ARMGNU)-objcopy ired03.gcc.elf ired03.gcc.bin -O binary


ired03.clang.bc : ired03.c
	clang $(LOPS) -c ired03.c -o ired03.clang.bc

ired03.clang.norm.bin : memmap vectors.o ired03.clang.bc
	llc $(LLCOPS) ired03.clang.bc -o ired03.clang.norm.s
	$(ARMGNU)-as ired03.clang.norm.s -o ired03.clang.norm.o
	$(ARMGNU)-ld -o ired03.clang.norm.elf -T memmap vectors.o ired03.clang.norm.o
	$(ARMGNU)-objdump -D ired03.clang.norm.elf > ired03.clang.norm.list
	$(ARMGNU)-objcopy ired03.clang.norm.elf ired03.clang.norm.bin -O binary

ired03.clang.opt.bin : memmap vectors.o ired03.clang.bc
	opt $(OOPS) ired03.clang.bc -o ired03.clang.opt.bc
	llc $(LLCOPS) ired03.clang.opt.bc -o ired03.clang.opt.s
	$(ARMGNU)-as ired03.clang.opt.s -o ired03.clang.opt.o
	$(ARMGNU)-ld -o ired03.clang.opt.elf -T memmap vectors.o ired03.clang.opt.o
	$(ARMGNU)-objdump -D ired03.clang.opt.elf > ired03.clang.opt.list
	$(ARMGNU)-objcopy ired03.clang.opt.elf ired03.clang.opt.bin -O binary




clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s

