#ifndef IDT_H
#define IDT_H

#include <stdint.h>


struct idtDesc
{
	uint16_t offset1;// off set bits 0-15
	uint16_t selector; //selector thats in our GDT
	uint8_t zero;// Does nothing, unused set to zero
	uint8_t typeAttr; //Descriptor type and attributes
	uint16_t offset2;  //offset bits 16-31
	
} __attribute__((packed));


struct idtrDesc
{
	uint16_t limit; // size of the descriptor table -1 
	uint32_t base; // Base address f the start of the interrupt descriptor table

} __attribute__((packed));



#endif
