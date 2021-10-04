#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"


uint16_t* videoMem = 0;
uint16_t terminalRow = 0;
uint16_t terminalCol = 0;

size_t strlen(const char * str)
{
	size_t len = 0;
	while(str[len])
	{
		len++;
	}
	return len;
}

uint16_t terminalMakeChar(uint8_t c , uint8_t color)
{
	return ((color << 8) | c);
}

void terminalPutChar(int x, int y, uint8_t c , uint8_t color)
{
	videoMem[(y*VGA_WIDTH) + x] = terminalMakeChar(c, color);
}


void terminalWriteChar(uint8_t c , uint8_t color)
{
	if(c == '\n')
	{
		terminalRow++;
		terminalCol = 0;
		return;
	}

	terminalPutChar(terminalCol, terminalRow, c, color);
	terminalCol++;
	if(terminalCol >= VGA_WIDTH)
	{
		terminalCol = 0;
		terminalRow++;
	}

}

void terminalInitialize()
{
	videoMem = (uint16_t *)VIDEO_MEM_LOCATION;
	terminalCol = terminalRow = 0;

	for(int y = 0; y < VGA_HEIGHT; y++)
	{
		for(int x = 0; x < VGA_WIDTH ; x++)
		{
			terminalPutChar(x,y,' ', 0);
		}
	}	
}


void print(const char * str)
{
	size_t len = strlen(str);
	for(int i = 0 ; i < len ; i++)
	{
		terminalWriteChar(str[i], WHITE_COLOR_CHAR);
	}

}


static struct paging4gbChunk* kernelChunk = 0;

void kernelMain()
{
	terminalInitialize();
	print("hello world \ntest");
	
	// Initiazize the global interrupt table

    kheapInit();
	idtInit();		


    // Setup paging
    kernelChunk = pagingNew4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    // Switch to kernel paging chunk
    pagingSwitch(paging4gbChunkGetDirectory(kernelChunk));

    // Enable paging
    enablePaging();
  	enableInterrupts();

}
