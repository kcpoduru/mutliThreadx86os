#include "idt.h"
#include "config.h"
#include "memory/memory.h"


struct idtDesc idtDescriptors[PK_OS]; 
struct idtrDesc idtrDescriptor;

void idtZero()
{
	print("Divide by zero error\n");
}

void idtSet(int interruptNo , void* address)
{
	struct idtDesc * desc = idtDescriptor[interruptNo];
	desc->offset1 = (uint32_t)address & 0x0000ffff;
    desc->selector = KERNEL_CODE_SELECTOR;
	desc->zero = 0x00;
	desc->typeAttr = 0xEE; // 47bit = 1(for intrpt present), 45, 46 bits = 3(user level, means user programs can call this 
	//interrupt, 44 bit = 0 for interrupts and trap gates, 40...43 bits = 0xE for 32 bit interrupt gate)
	desc->offset2 = (uint32_t)address >>16;


void idtInit()
{
	memset(idtDescriptors, 0, sizeof(idtDescriptors));
	idtrDescriptor.limit = sizeof(idtDescriptors) - 1;
	idtrDescriptor.base = idtDescriptors;

	idtSet(0, idtZero);
	
}
