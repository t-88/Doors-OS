INCLUDES = -Ikernel/drivers -Ikernel/memory -Icpu/ -Iklibc/ -Icpu/paging/
CC = gcc -g -m32 -fno-pie  -ffreestanding -fno-stack-protector -Wall


LD = ld -m elf_i386
KERNEL_ENTRY = 0x1600
C_SOURCE = $(wildcard kernel/*.c kernel/drivers/*.c kernel/memory/*.c klibc/*.c cpu/*.c cpu/paging/*.c)
OBJS = ${C_SOURCE:.c=.o}
ASM_OBJS = bootloader/kernel_entry.o  cpu/interrupt.o kernel/functions.o

OS_FILE = build/os_img.bin

all: run
run:  build/os_img.bin
	qemu-system-i386 -m 4G   -drive file=$(OS_FILE),if=floppy,format=raw  -vga std
build/os_img.bin: build/boot_sect.bin build/kernel.bin 
	cat $^ > $@
build/boot_sect.bin: bootloader/boot_sect.asm
	dd if=/dev/zero of=disk.img bs=512 count=4
	nasm  -fbin $< -o $@	
	dd if=$@ of=disk.img conv=notrunc
	nasm -f bin bootloader/memory_map_bs2.asm -o build/memory_map_bs2.bin    
	dd if=build/memory_map_bs2.bin of=disk.img bs=512 seek=1 conv=notrunc
	mv disk.img build/boot_sect.bin

build/kernel.bin: $(ASM_OBJS) $(OBJS) 
	$(LD)  -o $@ -Ttext $(KERNEL_ENTRY) $^ --oformat binary

%.o:%.c
	$(CC)  -o $@ -c $< ${INCLUDES}
%.o:%.asm
	nasm  -felf $< -o $@


kernel.elf: $(ASM_OBJS) $(OBJS)
	$(LD) -o $@ -Ttext $(KERNEL_ENTRY) $^ 
debug: build/os_img.bin kernel.elf
	qemu-system-i386 -m 4G  -s -S -drive file=$<,if=floppy,format=raw &
	gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"	


clean:
	rm  $(ASM_OBJS) $(OBJS)