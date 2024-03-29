[org 0x7c00]
[bits 16]
MMAP_ENTRY equ 0x1000
KERNEL_ENTRY equ 0x1600



start:
    mov [bootDrive] , dl ; storing boot drive

    ; init stack
    mov bp , 0x8000
    mov sp ,  bp

    ; get u how much memory in real mode, its 640kb
    ; the output is put into ax
    ; https://wiki.osdev.org/Detecting_Memory_(x86)#Detecting_Low_Memory
    ; clc
    ; int 0x12
    ; mov bx , ax
    ; call print_hex

    ; load 54 sector to mem for the kernel
    ; i change dh bc i want bigger kernel i really dont know what i am doing
    ; as i understad it when i load the kernel i have unlimited accses to memery at least 4gb
    ; BUT i always come to add more sectors so meh
    ;TODO: WTF IS THIS
    

    mov ax , 0
    mov es , ax 
    
    mov bx , MMAP_ENTRY
    mov dh , 53
    mov dl , [bootDrive]
    call load_kernel



    ; exit 16 bit mode
    mov bx, GOOD_BYE_16_BIT
    call print



    call MMAP_ENTRY



    cli
    lgdt [gdt_descriptor]
    mov eax , cr0    
    or eax , 1
    mov cr0 , eax    

    jmp CODE_SEG:init_pm
jmp $

%include "bootloader/gdt.asm"
%include "bootloader/utils.asm"
%include "bootloader/utils_pm.asm"




[bits 32]
init_pm:
    mov ax , DATA_SEG
    mov ds , ax
    mov ss , ax
    mov es , ax
    mov fs , ax
    mov gs , ax

    mov ebp , 0x90000
    mov esp , ebp


call main_pm

main_pm:
    mov edi , 0x00A0
    mov bx , HELLO_32_BIT
    call print_pm
    xor edi , edi

    call KERNEL_ENTRY

ret


GOOD_BYE_16_BIT db          "[Info] Exiting 16 bit mode", 10 , 13 ,0

bootDrive db 0
times 510 - ($ - $$) db 0 ; padding memory
dw 0xaa55

