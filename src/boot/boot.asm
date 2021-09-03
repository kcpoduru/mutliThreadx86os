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
    mov eax, 1 ; because the kernel is sector 1
    mov ecx, 100 ; and we want to load 100 sectors
    mov edi, 0x0100000 ; because we want to keep the kernel in 1 MB location
    call ataLbaRead ; we want to use this driver to copy the kernel from hard disk to memory
    jmp CODE_SEG:0x0100000


ataLbaRead: ; read ata driver on osdev docs
     mov ebx, eax, ; Backup the LBA
    ; Send the highest 8 bits of the lba to hard disk controller
    shr eax, 24
    or eax, 0xE0 ; Select the  master drive
    mov dx, 0x1F6
    out dx, al
    ; Finished sending the highest 8 bits of the lba

    ; Send the total sectors to read
    mov eax, ecx
    mov dx, 0x1F2
    out dx, al
    ; Finished sending the total sectors to read

    ; Send more bits of the LBA
    mov eax, ebx ; Restore the backup LBA
    mov dx, 0x1F3
    out dx, al
    ; Finished sending more bits of the LBA

    ; Send more bits of the LBA
    mov dx, 0x1F4
    mov eax, ebx ; Restore the backup LBA
    shr eax, 8
    out dx, al
    ; Finished sending more bits of the LBA

    ; Send upper 16 bits of the LBA
    mov dx, 0x1F5
    mov eax, ebx ; Restore the backup LBA
    shr eax, 16
    out dx, al
    ; Finished sending upper 16 bits of the LBA

    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

    ; Read all sectors into memory
.next_sector:
    push ecx

; Checking if we need to read, because of controller delay some times
.try_again:
    mov dx, 0x1f7
    in al, dx
    test al, 8
    jz .try_again

; We need to read 256 words at a time
    mov ecx, 256
    mov dx, 0x1F0
    rep insw
    pop ecx
    loop .next_sector
    ; End of reading sectors into memory
    ret

times 510-($ - $$) db 0 ; this will pad the complete binary with 510 bytes with zeros;  if the code uses up for eg 10 bytes 
; then the rest of the 500 bytes of the binary is padded with zeros 
dw 0xAA55 ;  boot signature bytes written.  BIOS will look at this signature at thik that this memory sector at location 512 is a bootable volume