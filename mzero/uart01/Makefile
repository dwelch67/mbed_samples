
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

LOPS = -Wall -m32 -emit-llvm

LLCOPS = -march=thumb

COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding

AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts

stuff = -O2 -std-compile-opts -strip-debug




all : uart01.gcc.thumb.bin uart01.clang.thumb.norm.bin uart01.clang.thumb.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart01.gcc.thumb.o : uart01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c uart01.c -o uart01.gcc.thumb.o

uart01.gcc.thumb.bin : memmap vectors.o uart01.gcc.thumb.o
	$(ARMGNU)-ld -o uart01.gcc.thumb.elf -T memmap vectors.o uart01.gcc.thumb.o
	$(ARMGNU)-objdump -D uart01.gcc.thumb.elf > uart01.gcc.thumb.list
	$(ARMGNU)-objcopy uart01.gcc.thumb.elf uart01.gcc.thumb.bin -O binary


uart01.clang.bc : uart01.c
	clang $(LOPS) -c uart01.c -o uart01.clang.bc

uart01.clang.thumb.norm.bin : memmap vectors.o uart01.clang.bc
	llc $(LLCOPS) uart01.clang.bc -o uart01.clang.thumb.norm.s
	$(ARMGNU)-as uart01.clang.thumb.norm.s -o uart01.clang.thumb.norm.o
	$(ARMGNU)-ld -o uart01.clang.thumb.norm.elf -T memmap vectors.o uart01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D uart01.clang.thumb.norm.elf > uart01.clang.thumb.norm.list
	$(ARMGNU)-objcopy uart01.clang.thumb.norm.elf uart01.clang.thumb.norm.bin -O binary

uart01.clang.thumb.opt.bin : memmap vectors.o uart01.clang.bc
	opt $(OOPS) uart01.clang.bc -o uart01.clang.thumb.opt.bc
	llc $(LLCOPS) uart01.clang.thumb.opt.bc -o uart01.clang.thumb.opt.s
	$(ARMGNU)-as uart01.clang.thumb.opt.s -o uart01.clang.thumb.opt.o
	$(ARMGNU)-ld -o uart01.clang.thumb.opt.elf -T memmap vectors.o uart01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D uart01.clang.thumb.opt.elf > uart01.clang.thumb.opt.list
	$(ARMGNU)-objcopy uart01.clang.thumb.opt.elf uart01.clang.thumb.opt.bin -O binary




clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s

