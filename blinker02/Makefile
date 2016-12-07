
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding
AOPS = --warn --fatal-warnings

all : blinker02.thumb.bin blinker02.thumb2.bin 

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

blinker02.thumb.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker02.c -o blinker02.thumb.o

blinker02.thumb2.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c blinker02.c -o blinker02.thumb2.o

blinker02.thumb.bin : flash.ld vectors.o blinker02.thumb.o
	$(ARMGNU)-ld -o blinker02.thumb.elf -T flash.ld vectors.o blinker02.thumb.o
	$(ARMGNU)-objdump -D blinker02.thumb.elf > blinker02.thumb.list
	$(ARMGNU)-objcopy blinker02.thumb.elf blinker02.thumb.bin -O binary

blinker02.thumb2.bin : flash.ld vectors.o blinker02.thumb2.o
	$(ARMGNU)-ld -o blinker02.thumb2.elf -T flash.ld vectors.o blinker02.thumb2.o
	$(ARMGNU)-objdump -D blinker02.thumb2.elf > blinker02.thumb2.list
	$(ARMGNU)-objcopy blinker02.thumb2.elf blinker02.thumb2.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

