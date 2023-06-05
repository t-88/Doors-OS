[bits 16]
load_kernel:    ; (dh , bx) 
    pusha       ;  dh => number of sectors to read starting from 1
    push dx     ;  bx => where to read those sectors
    
    
    mov al , dh ; read dh sectors 
    mov ch , 0  ; cylinder 0
    mov dh , 0  ; track 1
    mov cl , 2  ; sector 2

    mov ah , 2  ; read service
    int 0x13   


    pop dx
    jnc _check_number_of_sectors_read
        mov bx , KERNEL_LOAD_FAILED_CARRY
        call print
    jmp $
    
    _check_number_of_sectors_read:
        cmp dh , al  
        je _exit_routine_no_error
        mov bx , KERNEL_LOAD_FAILED_AL
        call print
    jmp $

    _exit_routine_no_error:
    popa
ret

print: ; (bx)
       ; bx => addr of first char in string
    push cx
    push ax
    xor cx , cx
    
    mov ah , 14
    _print_loop:
        mov al, byte [bx] 
        int 0x10
        inc bx
        cmp al , 0
    jne _print_loop
    
   pop ax 
   pop cx 
ret

print_hex: ; (bx)
           ; bx => hex code to be printed
    push bx
    push dx
    push cx
    push ax
    
    xor cx , cx
    mov ax , bx
    mov bx , 0x10
    _push_hex_loop:
        xor dx , dx
        div bx  
        inc cx
        push dx
        cmp ax , 0
    jne _push_hex_loop

    mov ah , 14
    
    mov al , "0" 
    int 0x10
    mov al , "x" 
    int 0x10
    _print_hex_loop:
        pop dx
        mov al , dl
        cmp al , 9
        jle __offset_hex_decimal
            add al , 55 ; 55 = "A" - "0"
        jmp __exit_offset
        __offset_hex_decimal:
            add al , "0"
        __exit_offset:
        int 0x10
    loop _print_hex_loop


    pop ax
    pop cx
    pop dx
    pop bx
ret