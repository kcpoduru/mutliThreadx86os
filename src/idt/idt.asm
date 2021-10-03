section .asm


extern int21hHandler
extern noInterruptHandler

global int21h
global idtLoad
global noInterrupt

global enableInterrupts
global disableInterrupts

enableInterrupts:
    sti
    ret

disableInterrupts:
    cli
    ret




idtLoad:
    push ebp
    mov ebp, esp
    mov ebx, [ebp+8]
    lidt [ebx]
    pop ebp
    ret

int21h:
    cli
    pushad
    call int21hHandler
    popad
    sti
    iret

noInterrupt:
    cli
    pushad
    call noInterruptHandler
    popad
    sti
    iret

