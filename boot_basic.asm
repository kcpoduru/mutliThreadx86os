ORG 0x7c00  ; location of the bootlaoder in memory
BITS 16 ; operation in 16 bit

start:
    mov ah, 0eh ; command arg to print  a char on the screen
    mov al, 'A' ; args moved to al to print  character 'A' on the screen
    mov bx, 0 ; set the page number
    int 0x10   ; interrupt number to invoke the BIOS routine to print the character on a screen

    jmp $ ; jump to itself ...below lines are not executable  instructions


message: db 'Hello  World!' , 0 ; null term at the end '0' , symbol hello world


times 510-($ - $$) db 0 ; this will pad the complete binary with 510 bytes with zeros;  if the code uses up for eg 10 bytes 
; then the rest of the 500 bytes of the binary is padded with zeros 
dw 0xAA55 ;  boot signature bytes written.  BIOS will look at this signature at thik that this memory sector at location 512 is a bootable volume