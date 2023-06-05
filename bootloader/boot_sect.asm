[org 0x7c00]
start:
    mov [bootDrive] , dl ; storing boot drive

    ; init stack
    mov bp , 0x8000
    mov sp ,  bp

    ; clear screen 
    mov ah , 0 
    mov al , 3
    int 0x10

    ; load 15 sector to mem
    mov bx , 0x9000
    mov dh , 3
    mov dl , [bootDrive]
    call load_kernel



    ; exit 16 bit mode
    mov bx, GOOD_BAY_16_BIT
    call print
jmp $


%include "bootloader/utils.asm"


KERNEL_LOAD_FAILED_CARRY db "[Error] Failed to load kernel carry is 1" , 0
KERNEL_LOAD_FAILED_AL db    "[Error] Failed to load kernel wrong number of sectors read", 0
GOOD_BAY_16_BIT db          "[Info] Exiting 16 bit mode", 0

bootDrive db ?


times 510 - ($ - $$) db 0 ; padding memory
dw 0xaa55


times 256 * 2 dw 0xaaaa