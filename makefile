INCLUDES = -Ikernel/drivers -Ikernel/memory -Icpu

# $@
# $<
# $^

all: run

build/kernel.o: kernel/kernel.c
	gcc -m32 -fno-pie  -o $@ -c $< $(INCLUDES)

build/kernel.bin: build/kernel_entry.o build/kernel.o
	ld -e main -m elf_i386   -o $@ -Ttext 0x1000 $^ --oformat binary

build/kernel_entry.o: bootloader/kernel_entry.asm
	nasm  -felf $< -o $@

build/boot_sect.bin: bootloader/boot_sect.asm
	nasm  -fbin $< -o $@

build/os_img.bin: build/boot_sect.bin build/kernel.bin 
	cat $^ > $@

run: build/os_img.bin
	qemu-system-i386  -drive file=$<,if=floppy,format=raw

clean:
	rm  build/*.o build/*.bin 