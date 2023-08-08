INCLUDES = -Ikernel/drivers -Ikernel/memory -Icpu/ -Iklibc/ -Icpu/paging/
CC = gcc -g -m32 -fno-pie  -ffreestanding -fno-stack-protector -Wall
LD = ld -m elf_i386

C_SOURCE = $(wildcard kernel/*.c kernel/drivers/*.c kernel/memory/*.c klibc/*.c cpu/*.c cpu/paging/*.c)
OBJS = ${C_SOURCE:.c=.o}
ASM_OBJS = bootloader/kernel_entry.o  cpu/interrupt.o

OS_FILE = build/os_img.bin

all: run
run:  build/os_img.bin
	qemu-system-i386   -drive file=$(OS_FILE),if=floppy,format=raw 
build/os_img.bin: build/boot_sect.bin build/kernel.bin 
	cat $^ > $@
build/boot_sect.bin: bootloader/boot_sect.asm
	nasm  -fbin $< -o $@	
build/kernel.bin: $(ASM_OBJS) $(OBJS) 
	$(LD)  -o $@ -Ttext 0x1000 $^ --oformat binary

%.o:%.c
	$(CC)  -o $@ -c $< ${INCLUDES}
%.o:%.asm
	nasm  -felf $< -o $@


kernel.elf: $(ASM_OBJS) $(OBJS)
	$(LD) -o $@ -Ttext 0x1000 $^ 
debug: build/os_img.bin kernel.elf
	qemu-system-i386  -s -S -drive file=$<,if=floppy,format=raw &
	gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"	


clean:
	rm  $(ASM_OBJS) $(OBJS)