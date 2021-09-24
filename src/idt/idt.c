#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "kernel.h"
#include "io/io.h"

struct idtDesc idtDescriptors[PKOS_TOTAL_INTERRUPTS]; 
struct idtrDesc idtrDescriptor;

extern void idtLoad(struct idtrDesc * ptr);
extern void int21h();
extern void noInterrupt();

void int21hHandler()
{
    print("Keyboard pressed!\n");
    outb(0x20, 0x20);
}

void noInterruptHandler()
{
    outb(0x20, 0x20);
}


void idtZero()
{
	print("Divide by zero error\n");
}

void idtSet(int interruptNo , void* address)
{
	struct idtDesc * desc = &idtDescriptors[interruptNo];
	desc->offset1 = (uint32_t)address & 0x0000ffff;
    desc->selector = KERNEL_CODE_SELECTOR;
	desc->zero = 0x00;
	desc->typeAttr = 0xEE; // 47bit = 1(for intrpt present), 45, 46 bits = 3(user level, means user programs can call this 
	//interrupt, 44 bit = 0 for interrupts and trap gates, 40...43 bits = 0xE for 32 bit interrupt gate)
	desc->offset2 = (uint32_t)address >>16;
}

void idtInit()
{
	memset(idtDescriptors, 0, sizeof(idtDescriptors));
	idtrDescriptor.limit = sizeof(idtDescriptors) - 1;
	idtrDescriptor.base = (uint32_t )idtDescriptors;

    for (int i = 0; i < PKOS_TOTAL_INTERRUPTS; i++)
    {
        idtSet(i, noInterrupt);
    }

    idtSet(0, idtZero);
    idtSet(0x21, int21h);


	//load the interrupt descriptor 
	idtLoad(&idtrDescriptor);
}
