
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 
GTHUMB = -mthumb
GTHUMB2 = -mthumb -mcpu=cortex-m3 -march=armv7-m

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts



all : all_gcc all_llvm


all_gcc : thread01.gcc.thumb.bin thread01.gcc.thumb2.bin


all_llvm : thread01.clang.thumb.norm.bin thread01.clang.thumb.opt.bin thread01.clang.thumb2.norm.bin thread01.clang.thumb2.opt.bin



vectors.o : vectors.s
	$(ARMGNU)-as $(AOPS) vectors.s -o vectors.o



thread01.gcc.thumb.bin : rommap vectors.o sched.c threada.c threadb.c threadc.c threadd.c
	$(ARMGNU)-gcc $(COPS) $(GTHUMB) -c sched.c -o sched.gcc.thumb.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB) -c threada.c -o threada.gcc.thumb.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB) -c threadb.c -o threadb.gcc.thumb.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB) -c threadc.c -o threadc.gcc.thumb.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB) -c threadd.c -o threadd.gcc.thumb.o
	$(ARMGNU)-ld -o thread01.gcc.thumb.elf -T rommap vectors.o sched.gcc.thumb.o threada.gcc.thumb.o threadb.gcc.thumb.o threadc.gcc.thumb.o threadd.gcc.thumb.o
	$(ARMGNU)-objdump -D thread01.gcc.thumb.elf > thread01.gcc.thumb.list
	$(ARMGNU)-objcopy thread01.gcc.thumb.elf thread01.gcc.thumb.bin -O binary

thread01.gcc.thumb2.bin : rommap vectors.o sched.c threada.c threadb.c threadc.c threadd.c
	$(ARMGNU)-gcc $(COPS) $(GTHUMB2) -c sched.c -o sched.gcc.thumb2.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB2) -c threada.c -o threada.gcc.thumb2.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB2) -c threadb.c -o threadb.gcc.thumb2.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB2) -c threadc.c -o threadc.gcc.thumb2.o
	$(ARMGNU)-gcc $(COPS) $(GTHUMB2) -c threadd.c -o threadd.gcc.thumb2.o
	$(ARMGNU)-ld -o thread01.gcc.thumb2.elf -T rommap vectors.o sched.gcc.thumb2.o threada.gcc.thumb2.o threadb.gcc.thumb2.o threadc.gcc.thumb2.o threadd.gcc.thumb2.o
	$(ARMGNU)-objdump -D thread01.gcc.thumb2.elf > thread01.gcc.thumb2.list
	$(ARMGNU)-objcopy thread01.gcc.thumb2.elf thread01.gcc.thumb2.bin -O binary




thread01.clang.bc : sched.c threada.c threadb.c threadc.c threadd.c
	clang $(LOPS) -c sched.c -o sched.clang.bc
	clang $(LOPS) -c threada.c -o threada.clang.bc
	clang $(LOPS) -c threadb.c -o threadb.clang.bc
	clang $(LOPS) -c threadc.c -o threadc.clang.bc
	clang $(LOPS) -c threadd.c -o threadd.clang.bc
	llvm-link sched.clang.bc threada.clang.bc threadb.clang.bc threadc.clang.bc threadd.clang.bc  -o thread01.clang.bc

thread01.clang.thumb.norm.bin : rommap vectors.o thread01.clang.bc
	llc $(LLCOPS) thread01.clang.bc -o thread01.clang.thumb.norm.s
	$(ARMGNU)-as thread01.clang.thumb.norm.s -o thread01.clang.thumb.norm.o
	$(ARMGNU)-ld -o thread01.clang.thumb.norm.elf -T rommap vectors.o thread01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D thread01.clang.thumb.norm.elf > thread01.clang.thumb.norm.list
	$(ARMGNU)-objcopy thread01.clang.thumb.norm.elf thread01.clang.thumb.norm.bin -O binary

thread01.clang.thumb.opt.bin : rommap vectors.o thread01.clang.bc
	opt $(OOPS) thread01.clang.bc -o thread01.clang.thumb.opt.bc
	llc $(LLCOPS) thread01.clang.thumb.opt.bc -o thread01.clang.thumb.opt.s
	$(ARMGNU)-as thread01.clang.thumb.opt.s -o thread01.clang.thumb.opt.o
	$(ARMGNU)-ld -o thread01.clang.thumb.opt.elf -T rommap vectors.o thread01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D thread01.clang.thumb.opt.elf > thread01.clang.thumb.opt.list
	$(ARMGNU)-objcopy thread01.clang.thumb.opt.elf thread01.clang.thumb.opt.bin -O binary

thread01.clang.thumb2.norm.bin : rommap vectors.o thread01.clang.bc
	llc $(LLCOPS2) thread01.clang.bc -o thread01.clang.thumb2.norm.s
	$(ARMGNU)-as thread01.clang.thumb2.norm.s -o thread01.clang.thumb2.norm.o
	$(ARMGNU)-ld -o thread01.clang.thumb2.norm.elf -T rommap vectors.o thread01.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D thread01.clang.thumb2.norm.elf > thread01.clang.thumb2.norm.list
	$(ARMGNU)-objcopy thread01.clang.thumb2.norm.elf thread01.clang.thumb2.norm.bin -O binary

thread01.clang.thumb2.opt.bin : rommap vectors.o thread01.clang.bc
	opt $(OOPS) thread01.clang.bc -o thread01.clang.thumb2.opt.bc
	llc $(LLCOPS2) thread01.clang.thumb2.opt.bc -o thread01.clang.thumb2.opt.s
	$(ARMGNU)-as thread01.clang.thumb2.opt.s -o thread01.clang.thumb2.opt.o
	$(ARMGNU)-ld -o thread01.clang.thumb2.opt.elf -T rommap vectors.o thread01.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D thread01.clang.thumb2.opt.elf > thread01.clang.thumb2.opt.list
	$(ARMGNU)-objcopy thread01.clang.thumb2.opt.elf thread01.clang.thumb2.opt.bin -O binary


clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s


