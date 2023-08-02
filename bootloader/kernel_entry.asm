global _start;
_start:
[bits 32]
    [extern kernel_main]
    call kernel_main
    jmp $ ; stay here
