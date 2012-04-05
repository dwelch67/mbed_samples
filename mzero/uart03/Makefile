
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm -ccc-host-triple $(ARMGNU)

LLCOPS = -march=thumb -mtriple=$(ARMGNU)

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts

stuff = -O2 -std-compile-opts -strip-debug




all : uart03.gcc.thumb.bin uart03.clang.thumb.norm.bin uart03.clang.thumb.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart03.gcc.thumb.o : uart03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c uart03.c -o uart03.gcc.thumb.o

uart03.gcc.thumb.bin : memmap vectors.o uart03.gcc.thumb.o
	$(ARMGNU)-ld -o uart03.gcc.thumb.elf -T memmap vectors.o uart03.gcc.thumb.o
	$(ARMGNU)-objdump -D uart03.gcc.thumb.elf > uart03.gcc.thumb.list
	$(ARMGNU)-objcopy uart03.gcc.thumb.elf uart03.gcc.thumb.bin -O binary


uart03.clang.bc : uart03.c
	clang $(LOPS) -c uart03.c -o uart03.clang.bc

uart03.clang.thumb.norm.bin : memmap vectors.o uart03.clang.bc
	llc $(LLCOPS) uart03.clang.bc -o uart03.clang.thumb.norm.s
	$(ARMGNU)-as uart03.clang.thumb.norm.s -o uart03.clang.thumb.norm.o
	$(ARMGNU)-ld -o uart03.clang.thumb.norm.elf -T memmap vectors.o uart03.clang.thumb.norm.o
	$(ARMGNU)-objdump -D uart03.clang.thumb.norm.elf > uart03.clang.thumb.norm.list
	$(ARMGNU)-objcopy uart03.clang.thumb.norm.elf uart03.clang.thumb.norm.bin -O binary

uart03.clang.thumb.opt.bin : memmap vectors.o uart03.clang.bc
	opt $(OOPS) uart03.clang.bc -o uart03.clang.thumb.opt.bc
	llc $(LLCOPS) uart03.clang.thumb.opt.bc -o uart03.clang.thumb.opt.s
	$(ARMGNU)-as uart03.clang.thumb.opt.s -o uart03.clang.thumb.opt.o
	$(ARMGNU)-ld -o uart03.clang.thumb.opt.elf -T memmap vectors.o uart03.clang.thumb.opt.o
	$(ARMGNU)-objdump -D uart03.clang.thumb.opt.elf > uart03.clang.thumb.opt.list
	$(ARMGNU)-objcopy uart03.clang.thumb.opt.elf uart03.clang.thumb.opt.bin -O binary




clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s

