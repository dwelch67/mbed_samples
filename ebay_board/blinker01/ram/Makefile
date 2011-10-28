
#ARMGNU=arm-thumb-elf
#ARMGNU=arm-none-linux-gnueabi
ARMGNU?=arm-none-eabi

all : blinker01.bin

blinker01.bin : blinker01.s memmap
	$(ARMGNU)-as  blinker01.s -o blinker01.o
	$(ARMGNU)-ld -o blinker01.elf blinker01.o -T memmap
	$(ARMGNU)-objdump -D blinker01.elf > blinker01.list
	$(ARMGNU)-objcopy blinker01.elf blinker01.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

