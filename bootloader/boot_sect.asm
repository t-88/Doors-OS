[org 0x7c00]
[bits 16]
KERNEL_ENTRY equ 0x1000

start:
    mov [bootDrive] , dl ; storing boot drive

    ; init stack
    mov bp , 0x8000
    mov sp ,  bp

    ; clear screen 
    mov ah , 0 
    mov al , 3
    int 0x10

    ; load 15 sector to mem for the kernel
    mov bx , KERNEL_ENTRY
    mov dh , 32
    mov dl , [bootDrive]
    call load_kernel


    ; exit 16 bit mode
    mov bx, GOOD_BYE_16_BIT
    call print



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
    jmp $
ret


KERNEL_LOAD_FAILED_CARRY db "[Error] Failed to load kernel carry is 1" , 0
KERNEL_LOAD_FAILED_AL db    "[Error] Failed to load kernel wrong number of sectors read", 0
GOOD_BYE_16_BIT db          "[Info] Exiting 16 bit mode", 0
HELLO_32_BIT db          "[Info] We are Now in 32 bit mode", 0

bootDrive db 0
times 510 - ($ - $$) db 0 ; padding memory
dw 0xaa55
