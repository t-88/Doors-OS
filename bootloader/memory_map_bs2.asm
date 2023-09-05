
[org 0x1000]
[bits 16]


struc	MemoryMapEntry
	.baseAddress	resq	1	; base address of address range
	.length		resq	1	    ; length of address range in bytes
	.type		resd	1	    ; type of address range
	.acpi_null	resd	1	    ; reserved
endstruc



section .data
    STR_MMAP_INIT db "[INFO] Initializing mmap" ,10 , 13,  0
    STR_MMAP_DONE db "[INFO] Done mmaping" , 10 , 13 , 0
    STR_MMAP_RETURN db "[INFO] Returning to bootloader" , 10 , 13 , 0
    STR_MMAP_FAILURE db "[ERROR] Failed to mmap" , 10 , 13 , 0




section .text
mmap_entry:
    mov bx , STR_MMAP_INIT
    call print



    mov ax , 0x120
    mov es , ax

    ; init
    mov bp , 0
    mov edi , 0
    mov ebx , 0

    .next_entry:
        mov ecx , 24
        mov edx , 0x534D4150
        mov eax , 0x0000E820
        int 0x15
        cmp  eax, 0x534D4150
        jne .ERROR 
        
    .start:
        jcxz .skip_entry ; if cx = 0, means that entry is empty
    .notext:
        mov ecx , [es:di + MemoryMapEntry.length]
        test ecx, ecx
        jne .good_entry
        mov ecx , [es:di + MemoryMapEntry.length + 4]
        jecxz .skip_entry

    .good_entry:
        inc bp  
        add di, 24

    .skip_entry:
        cmp ebx, 0  ; next offset means done loading entries    
        je .done

        push ebx 
        mov bx , bp
        call print_hex
        pop ebx

        jmp .next_entry


    .done:
        mov bx, STR_MMAP_DONE
        call print

        mov bx, bp
        call print_hex



        ; mov bx, [es:0000]
        ; call print_hex
        ; jmp $

        jmp .return_to_bootloader
 
        ; mov di, 0
        ; mov bx , [es:di]
        ; call print_hex

    .ERROR:
        mov bx, STR_MMAP_FAILURE
        call print
        jmp $


    .return_to_bootloader:
        ; jmp $
        ret

%include "bootloader/utils.asm"




