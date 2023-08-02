INCLUDES = -Ikernel/drivers -Ikernel/memory -Icpu -Iutils
CC = gcc -g -m32 -fno-pie -fno-stack-protector
LD = ld -m elf_i386

OBJ = build/kernel_entry.o  build/idt.o build/isr.o     build/port.o build/screen.o build/kmemory.o   build/kernel.o   build/interrupt.o build/sys_std.o build/pic.o build/timer.o build/keyboard.o
C_SOURCE = $(wildcard kernel/*.c kernel/drivers/*.c kernel/memory/*.c utils/*.c cpu/*.c)
OBJS = ${C_SOURCE:.c=.o bootloader/kernel_entry.o  cpu/interrupt.o}

OS_FILE = build/os_img.bin

all: run
run:  build/os_img.bin
	qemu-system-i386  -drive file=$(OS_FILE),if=floppy,format=raw 
build/os_img.bin: build/boot_sect.bin build/kernel.bin 
	cat $^ > $@
build/boot_sect.bin: bootloader/boot_sect.asm
	nasm  -fbin $< -o $@	
build/kernel.bin:  $(OBJS) 
	$(LD)  -o $@ -Ttext 0x1000 $^ --oformat binary

%.o:%.c
	$(CC)  -o $@ -c $< ${INCLUDES}
%.o:%.asm
	nasm  -felf $< -o $@


kernel.elf: $(OBJ)
	$(LD) -o $@ -Ttext 0x1000 $^ 
debug: build/os_img.bin kernel.elf
	qemu-system-i386  -s -S -drive file=$<,if=floppy,format=raw 


clean:
	rm   $(OBJS)