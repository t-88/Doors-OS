INCLUDES = -Ikernel/drivers -Ikernel/memory -Icpu -Iutils

# $@
# $<
# $^

all: run

build/kernel.o: kernel/kernel.c
	gcc -g -m32 -fno-pie  -o $@ -c $< ${INCLUDES}
build/isr.o: cpu/isr.c
	gcc -g -m32 -fno-pie  -o $@ -c $< ${INCLUDES}
build/port.o: kernel/drivers/port.c
	gcc -g -m32 -fno-pie  -o $@ -c $< ${INCLUDES}
build/kmemory.o: kernel/memory/kmemory.c
	gcc -g -m32 -fno-pie  -o $@ -c $< ${INCLUDES}
build/screen.o: kernel/drivers/screen.c
	gcc -g -m32 -fno-pie  -o $@ -c $< ${INCLUDES}
build/idt.o: cpu/idt.c
	gcc -g -m32 -fno-pie  -o $@ -c $< ${INCLUDES}


build/kernel.bin: build/kernel_entry.o build/idt.o build/isr.o     build/port.o build/screen.o build/kmemory.o  build/kernel.o   build/interrupt.o
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


kernel.elf: build/kernel_entry.o build/idt.o build/isr.o     build/port.o build/screen.o build/kmemory.o  build/kernel.o   build/interrupt.o
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ 

debug: build/os_img.bin kernel.elf
	qemu-system-i386  -s -S -drive file=$<,if=floppy,format=raw 


clean:
	rm  build/*.o build/*.bin 