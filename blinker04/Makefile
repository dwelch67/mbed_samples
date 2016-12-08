
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

COPS2 = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=cortex-m3 -march=armv7-m

all : blinker04.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

blinker04.o : blinker04.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker04.c -o blinker04.o

blinker04.bin : flash.ld vectors.o blinker04.o
	$(ARMGNU)-ld -o blinker04.elf -T flash.ld vectors.o blinker04.o
	$(ARMGNU)-objdump -D blinker04.elf > blinker04.list
	$(ARMGNU)-objcopy blinker04.elf blinker04.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

