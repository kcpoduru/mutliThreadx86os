[BITS 32]

global _start
global problem
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

  ; Remap the master PIC
    mov al, 00010001b
    out 0x20, al ; Tell master PIC

    mov al, 0x20 ; Interrupt 0x20 is where master ISR should start
    out 0x21, al

    mov al, 00000001b
    out 0x21, al
    ; End remap of the master PIC
    
    call kernelMain

    jmp $


problem:
    mov eax, 0
    div eax


times 512-($ - $$) db 0 ; for alignment issues to be avioded