[bits 16]
gdt_start:
    gdt_null: ; null seg
        dd 0
        dd 0
    gdt_code:
        dw 0xffff
        dw 0
        db 0         
        db 10011010b 
        db 11001111b 
        db 0 
        
    gdt_data:
        dw 0xffff
        dw 0
        db 0         
        db 10010010b 
        db 11001111b 
        db 0         

    gdt_end:

    gdt_descriptor:
        dw gdt_end - gdt_start - 1
        dd gdt_start
    
    CODE_SEG equ gdt_code - gdt_start 
    DATA_SEG equ gdt_data - gdt_start 

