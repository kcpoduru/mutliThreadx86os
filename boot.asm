ORG 0x7c00  ; location of the bootlaoder in memory
BITS 16 ; operation in 16 bit

start:
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