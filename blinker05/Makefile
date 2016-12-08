
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

COPS2 = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=cortex-m3 -march=armv7-m

all : blinker05.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

blinker05.o : blinker05.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker05.c -o blinker05.o

blinker05.bin : flash.ld vectors.o blinker05.o
	$(ARMGNU)-ld -o blinker05.elf -T flash.ld vectors.o blinker05.o
	$(ARMGNU)-objdump -D blinker05.elf > blinker05.list
	$(ARMGNU)-objcopy blinker05.elf blinker05.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

