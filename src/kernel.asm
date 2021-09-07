[BITS 32]

section .asm
global _start
extern kernelMain

CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp
; Enable the a20 line refet to os.dev A20 line
    in al, 0x92  ; in refers to getting the value from the system bus
    or al, 2
    out 0x92, al ; out refers to writing value to system bus
    call kernelMain
    jmp $



times 512-($ - $$) db 0 ; for alignment issues to be avioded