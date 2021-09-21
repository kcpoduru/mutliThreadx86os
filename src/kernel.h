#ifndef KERNEL_H
#define KERNEL_H

#include "./idt/idt.h"

void kernelMain();
void print(const char * str);

//Text Mode Print macros
#define VIDEO_MEM_LOCATION 			(0xB8000)
#define VGA_WIDTH 			    	80
#define VGA_HEIGHT			    	20
#define WHITE_COLOR_CHAR 			15

#endif
