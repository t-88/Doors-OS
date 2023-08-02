; cpu excutes code line by line so having a entry 
; is telling the cpu to jump to main function

global _start;
_start:
[bits 32]
    [extern main]
    call main
    jmp $ ; stay here
