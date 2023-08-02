INCLUDES = -Ikernel/drivers -Ikernel/memory -Icpu -Iutils
CC = gcc -g -m32 -fno-pie -fno-stack-protector
OBJ = build/kernel_entry.o  build/idt.o build/isr.o     build/port.o build/screen.o build/kmemory.o  build/kernel.o   build/interrupt.o build/sys_std.o build/pic.o

# $@
# $<
# $^

all: run

build/kernel.o: kernel/kernel.c
	$(CC)  -o $@ -c $< ${INCLUDES}
build/isr.o: cpu/isr.c
	$(CC)  -o $@ -c $< ${INCLUDES}
build/port.o: kernel/drivers/port.c
	$(CC)  -o $@ -c $< ${INCLUDES}
build/kmemory.o: kernel/memory/kmemory.c
	$(CC)  -o $@ -c $< ${INCLUDES}
build/screen.o: kernel/drivers/screen.c
	$(CC)  -o $@ -c $< ${INCLUDES}
build/idt.o: cpu/idt.c
	$(CC)  -o $@ -c $< ${INCLUDES}
build/sys_std.o: utils/sys_std.c
	$(CC)  -o $@ -c $< ${INCLUDES}
build/pic.o: cpu/pic.c
	$(CC)  -o $@ -c $< ${INCLUDES}


build/kernel.bin: $(OBJ)
	ld -e main -m elf_i386   -o $@ -Ttext 0x1000 $^ --oformat binary

build/interrupt.o: cpu/interrupt.asm
	nasm  -felf $< -o $@

build/kernel_entry.o: bootloader/kernel_entry.asm
	nasm  -felf $< -o $@

build/boot_sect.bin: bootloader/boot_sect.asm
	nasm  -fbin $< -o $@

build/os_img.bin: build/boot_sect.bin build/kernel.bin 
	cat $^ > $@

run: build/os_img.bin
	qemu-system-i386  -drive file=$<,if=floppy,format=raw 


kernel.elf: $(OBJ)
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ 

debug: build/os_img.bin kernel.elf
	qemu-system-i386  -s -S -drive file=$<,if=floppy,format=raw 


clean:
	rm  build/*.o build/*.bin 