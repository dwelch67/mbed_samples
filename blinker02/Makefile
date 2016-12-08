
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

COPS2 = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=cortex-m3 -march=armv7-m

all : blinker02.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

blinker02.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker02.c -o blinker02.o

blinker02.bin : flash.ld vectors.o blinker02.o
	$(ARMGNU)-ld -o blinker02.elf -T flash.ld vectors.o blinker02.o
	$(ARMGNU)-objdump -D blinker02.elf > blinker02.list
	$(ARMGNU)-objcopy blinker02.elf blinker02.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

