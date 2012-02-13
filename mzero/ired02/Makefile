
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm

LLCOPS = -march=thumb

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts

stuff = -O2 -std-compile-opts -strip-debug




all : ired02.gcc.bin ired02.clang.norm.bin ired02.clang.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

ired02.gcc.o : ired02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c ired02.c -o ired02.gcc.o

ired02.gcc.bin : memmap vectors.o ired02.gcc.o
	$(ARMGNU)-ld -o ired02.gcc.elf -T memmap vectors.o ired02.gcc.o
	$(ARMGNU)-objdump -D ired02.gcc.elf > ired02.gcc.list
	$(ARMGNU)-objcopy ired02.gcc.elf ired02.gcc.bin -O binary


ired02.clang.bc : ired02.c
	clang $(LOPS) -c ired02.c -o ired02.clang.bc

ired02.clang.norm.bin : memmap vectors.o ired02.clang.bc
	llc $(LLCOPS) ired02.clang.bc -o ired02.clang.norm.s
	$(ARMGNU)-as ired02.clang.norm.s -o ired02.clang.norm.o
	$(ARMGNU)-ld -o ired02.clang.norm.elf -T memmap vectors.o ired02.clang.norm.o
	$(ARMGNU)-objdump -D ired02.clang.norm.elf > ired02.clang.norm.list
	$(ARMGNU)-objcopy ired02.clang.norm.elf ired02.clang.norm.bin -O binary

ired02.clang.opt.bin : memmap vectors.o ired02.clang.bc
	opt $(OOPS) ired02.clang.bc -o ired02.clang.opt.bc
	llc $(LLCOPS) ired02.clang.opt.bc -o ired02.clang.opt.s
	$(ARMGNU)-as ired02.clang.opt.s -o ired02.clang.opt.o
	$(ARMGNU)-ld -o ired02.clang.opt.elf -T memmap vectors.o ired02.clang.opt.o
	$(ARMGNU)-objdump -D ired02.clang.opt.elf > ired02.clang.opt.list
	$(ARMGNU)-objcopy ired02.clang.opt.elf ired02.clang.opt.bin -O binary




clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s

