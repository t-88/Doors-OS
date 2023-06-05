main:
	nasm -f bin bootloader/boot_sect.asm -o build/boot_sect.bin
	qemu-system-x86_64 -drive file=build/boot_sect.bin,format=raw,if=floppy