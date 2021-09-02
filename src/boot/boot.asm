ORG 0x7c00;
BITS 16 ; operation in 16 bit

CODE_SEG equ gdtCode - gdtStart
DATA_SEG equ gdtData - gdtStart


_start:
    jmp short start ;BIOS parameter block first and second instructions must be this and next instruction
    nop


times 33 db 0 ; padding 33 bytes of the binary with 0 for BIOS parameter block

start:
    jmp 0:step2 ; this will make a jump tp 0x7c0 segment and step2 offset
    

 step2:   
    cli ; clear interrupts flags
    ; enterring the  critical appleication area so diabling the interrupts
    mov ax, 0x00 ; location where the bios will copy our bootloader to memory  
    mov ds, ax ; setting up the data segment location to  0x7c0
    mov es, ax  ; setting up the  extra segment to same location
    mov ss, ax ; setting  up to 0x00
    mov sp, 0x7c00 ; setting up the stack pointer to 0x7c00 as it grows downwards to 0 full descending
    ; critical secti0on ended to enabling inteerupts
    sti ; enables interrupts


; jumping to protected mode https://wiki.osdev.org/Protected_Mode
.loadProtected:
    cli 
    lgdt[gdtDescriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:load32

; GDT

gdtStart:
gdtNull:
    dd 0x0
    dd 0x0


; offset 0x8
gdtCode:                ;CS should point to this
    dw 0xffff           ;Segment limit first 0-15 bits
    dw 0                ;Base first 0-15 bits 
    db 0                ;Base 16-23 bits
    db 0x9a             ; Access Byte
    db 11001111b        ; High 4 bit flags and the low 4 bit flags
    db 0                ; Base 24-31 bits


; offset 0x10
gdtData:               ; DS, SS,ES, FS, GS
    dw 0xffff           ;Segment limit first 0-15 bits
    dw 0                ;Base first 0-15 bits 
    db 0                ;Base 16-23 bits
    db 0x92            ; Access Byte
    db 11001111b        ; High 4 bit flags and the low 4 bit flags
    db 0                ; Base 24-31 bits


gdtEnd:

gdtDescriptor:
    dw gdtEnd - gdtStart-1
    dd gdtStart


[BITS 32]
load32:
    mov ax, DATA_SEG
    ;mov ds, ax
    ;mov es, ax
    ;mov fs, ax
    ;mov gs, ax
    ;mov ss, ax
    ;mov ebp, 0x00200000
    ;mov esp, ebp
    jmp $


times 510-($ - $$) db 0 ; this will pad the complete binary with 510 bytes with zeros;  if the code uses up for eg 10 bytes 
; then the rest of the 500 bytes of the binary is padded with zeros 
dw 0xAA55 ;  boot signature bytes written.  BIOS will look at this signature at thik that this memory sector at location 512 is a bootable volume