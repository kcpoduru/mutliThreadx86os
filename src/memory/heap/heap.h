#ifndef HEAP_H
#define HEAP_H
#include "config.h"
#include <stdint.h>
#include <stddef.h>

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN                    0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE                     0x00

#define HEAP_BLOCK_HAS_NEXT                             0b10000000
#define HEAP_BLOCK_IS_FIRST                             0b01000000


typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;

struct heapTable
{
    HEAP_BLOCK_TABLE_ENTRY* entries;
    size_t total;
};


struct heap
{
    struct heapTable* table;

    // Start address of the heap data pool
    void* saddr;
};

int heapCreate(struct heap* heap, void* ptr, void* end, struct heapTable* table);
void* heapMalloc(struct heap* heap, size_t size);
void heapFree(struct heap* heap, void* ptr);

#endif