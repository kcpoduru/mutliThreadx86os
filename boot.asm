ORG 0x00;
BITS 16 ; operation in 16 bit
_start:
    jmp short start ;BIOS parameter block first and second instructions must be this and next instruction
    nop


times 33 db 0 ; padding 33 bytes of the binary with 0 for BIOS parameter block

start:
    jmp 0x7c0:step2 ; this will make a jump tp 0x7c0 segment and step2 offset
    

 step2:   
    cli ; clear interrupts flags
    ; enterring the  critical appleication area so diabling the interrupts
    mov ax, 0x7c0 ; location where the bios will copy our bootloader to memory  
    mov ds, ax ; setting up the data segment location to  0x7c0
    mov es, ax  ; setting up the  extra segment to same location
    mov ax, 0x00 ; setting up the stack location at start of the ram 
    mov ss, ax ; setting  up to 0x00
    mov sp, 0x7c00 ; setting up the stack pointer to 0x7c00 as it grows downwards to 0 full descending
    ; critical secti0on ended to enabling inteerupts
    sti ; enables interrupts
    mov si, message ; move the message lable address to si register
    call print ; fun call
    jmp $ ; stay here infinitely like while 1



print:
    mov bx, 0 ; set the page number

.loop:    ; sublable inside print
    lodsb ; this instruction loads the value from address stored in si register and copies it to al register, if called again it will increment the address and loads again to al register
    cmp al , 0 ; comparing if we read till end of hello world. Null terminated string
    je .done ; if equal flag set then we are done
    call printChar ; else printchar. the print char routine takes the char present in al reg and prints it on screen 
    jmp .loop

.done:
    ret


printChar:
    mov ah, 0eh ; command arg to print  a char on the screen
    int 0x10   ; interrupt number to invoke the BIOS routine like a syscall for BIOS
    ret



message: db 'Hello  World!' , 0 ; null term at the end '0' , symbol hello world


times 510-($ - $$) db 0 ; this will pad the complete binary with 510 bytes with zeros;  if the code uses up for eg 10 bytes 
; then the rest of the 500 bytes of the binary is padded with zeros 
dw 0xAA55 ;  boot signature bytes written.  BIOS will look at this signature at thik that this memory sector at location 512 is a bootable volume