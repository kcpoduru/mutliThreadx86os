#include "heap.h"
#include "kernel.h"
#include "status.h"
#include "memory/memory.h"
#include <stdbool.h>

static int heapValidateTable(void* ptr, void* end, struct heapTable* table)
{
    int res = 0;

    size_t tableSize = (size_t)(end - ptr);
    size_t totalBlocks = tableSize / PKOS_HEAP_BLOCK_SIZE;
    if (table->total != totalBlocks)
    {
        res = -EINVARG;
        goto out;
    }

out:
    return res;
}

static bool heapValidateAlignment(void* ptr)
{
    return ((unsigned int)ptr % PKOS_HEAP_BLOCK_SIZE) == 0;
}

int heapCreate(struct heap* heap, void* ptr, void* end, struct heapTable* table)
{
    int res = 0;

    if (!heapValidateAlignment(ptr) || !heapValidateAlignment(end))
    {
        res = -EINVARG;
        goto out;
    }

    memset(heap, 0, sizeof(struct heap));
    heap->saddr = ptr;
    heap->table = table;

    res = heapValidateTable(ptr, end, table);
    if (res < 0)
    {
        goto out;
    }

    size_t tableSize = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total;
    memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, tableSize);

out:
    return res;
}

static uint32_t heapAlignValueToUpper(uint32_t val)
{
    if ((val % PKOS_HEAP_BLOCK_SIZE) == 0)
    {
        return val;
    }

    val = (val - ( val % PKOS_HEAP_BLOCK_SIZE));
    val += PKOS_HEAP_BLOCK_SIZE;
    return val;
}

static int heapGetEntryType(HEAP_BLOCK_TABLE_ENTRY entry)
{
    return entry & 0x0f;
}

int heapGetStartBlock(struct heap* heap, uint32_t totalBlocks)
{
    struct heapTable* table = heap->table;
    int bc = 0;
    int bs = -1;

    for (size_t i = 0; i < table->total; i++)
    {
        if (heapGetEntryType(table->entries[i]) != HEAP_BLOCK_TABLE_ENTRY_FREE)
        {
            bc = 0;
            bs = -1;
            continue;
        }

        // If this is the first block
        if (bs == -1)
        {
            bs = i;
        }
        bc++;
        if (bc == totalBlocks)
        {
            break;
        }
    }

    if (bs == -1)
    {
        return -ENOMEM;
    }

    return bs;

}

void* heapBlockToAddress(struct heap* heap, int block)
{
    return heap->saddr + (block * PKOS_HEAP_BLOCK_SIZE);
}

void heapmarkblockstaken(struct heap* heap, int startblock, int totalBlocks)
{
    int endblock = (startblock + totalBlocks)-1;

    HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
    if (totalBlocks > 1)
    {
        entry |= HEAP_BLOCK_HAS_NEXT;
    }

    for (int i = startblock; i <= endblock; i++)
    {
        heap->table->entries[i] = entry;
        entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
        if (i != endblock -1)
        {
            entry |= HEAP_BLOCK_HAS_NEXT;
        }
    }
}

void* heapMallocBlocks(struct heap* heap, uint32_t totalBlocks)
{
    void* address = 0;

    int startblock = heapGetStartBlock(heap, totalBlocks);
    if (startblock < 0)
    {
        goto out;
    }

    address = heapBlockToAddress(heap, startblock);

    // Mark the blocks as taken
    heapmarkblockstaken(heap, startblock, totalBlocks);

out:
    return address;
}

void heapMarkBlocksFree(struct heap* heap, int startingBlock)
{
    struct heapTable* table = heap->table;
    for (int i = startingBlock; i < (int)table->total; i++)
    {
        HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];
        table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;
        if (!(entry & HEAP_BLOCK_HAS_NEXT))
        {
            break;
        }
    }
}

int heapAddressToBlock(struct heap* heap, void* address)
{
    return ((int)(address - heap->saddr)) / PKOS_HEAP_BLOCK_SIZE;
}

void* heapMalloc(struct heap* heap, size_t size)
{
    size_t alignedSize = heapAlignValueToUpper(size);
    uint32_t totalBlocks = alignedSize / PKOS_HEAP_BLOCK_SIZE;
    return heapMallocBlocks(heap, totalBlocks);
}

void heapFree(struct heap* heap, void* ptr)
{
    heapMarkBlocksFree(heap, heapAddressToBlock(heap, ptr));
} 