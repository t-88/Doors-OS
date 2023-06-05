qemu:
	nasm -f bin bootloader/boot_sect.asm -o build/boot_sect.bin
	nasm -f elf bootloader/kernel_entry.asm -o build/kernel_entry.o


	# i know i am building a 32 bit sys but i added 
	# so many flags that my head will explode
	# i need to wrap my head around this stuff
	
	gcc -ffreestanding -m32 -fno-pie  -Ikernel/drivers -c  kernel/kernel.c  -o build/kernel.o
	gcc -ffreestanding -m32 -fno-pie  -Ikernel/drivers -c  kernel/screen.c  -o build/screen.o


	ld -m elf_i386 -o build/kernel.bin -e main -Ttext 0x1000 build/kernel_entry.o build/kernel.o build/screen.o --oformat binary   

	cat build/boot_sect.bin build/kernel.bin > build/DoorsOS
	qemu-system-x86_64 -drive file=build/DoorsOS,format=raw,if=floppy

	