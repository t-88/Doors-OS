[bits 32]
print_pm: ; (bx , edi)
          ; bx offset of string to printed
          ; position in screen
pusha
    mov ah , 0x0f
    _print_pm:
        mov al , byte [bx]

        mov [0xb8000 + edi] , ax

        add bx , 1
        add edi , 2
        cmp al , 0
    jne _print_pm

popa
ret
