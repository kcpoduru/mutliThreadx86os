#include "kheap.h"
#include "heap.h"
#include "config.h"
#include "kernel.h"

struct heap kernelHeap;
struct heapTable kernelHeapTable;

void kheapInit()
{
    int  totalTableEntries = PKOS_HEAP_SIZE_BYTES / PKOS_HEAP_BLOCK_SIZE;
    kernelHeapTable.entries = (HEAP_BLOCK_TABLE_ENTRY*)(PKOS_HEAP_TABLE_ADDRESS);
    kernelHeapTable.total =  totalTableEntries;

    void* end = (void*)(PKOS_HEAP_ADDRESS + PKOS_HEAP_SIZE_BYTES);
    int res = heapCreate(&kernelHeap, (void*)(PKOS_HEAP_ADDRESS), end, &kernelHeapTable);
    if (res < 0)
    {
        print("Failed to create heap\n");
    }

}

void* kmalloc(size_t size)
{
    return heapMalloc(&kernelHeap, size);
}

void kfree(void* ptr)
{
    heapFree(&kernelHeap, ptr);
} 