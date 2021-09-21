#include "memory.h"



void* memset(void * ptr , int c , size_t size)
{
	char *cPtr = (char *)ptr;
	
	for(int i = 0; i < c ; i++)
	{
		cPtr[i]  = (char)c;
	}
	return ptr;
}
