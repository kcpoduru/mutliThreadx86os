#include "paging.h"
#include "memory/heap/kheap.h"
#include "status.h"

void pagingLoadDirectory(uint32_t* directory);

static uint32_t *currentDirectory = 0;

struct paging4gbChunk* pagingNew4gb(uint8_t flags)
{
    uint32_t * directory = (uint32_t *)kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
    if(directory == NULL) goto error;
    uint32_t offset = 0;

    for(int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++)
    {
        uint32_t * pageTable = (uint32_t *)kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
        if(pageTable == NULL) goto error;
        for(int j = 0; j < PAGING_TOTAL_ENTRIES_PER_TABLE ; j++)
        {
            pageTable[j] = offset | flags;
            offset+=PAGING_PAGE_SIZE;
        }
        directory[i] = (uint32_t)pageTable | flags |  PAGING_IS_WRITEABLE;
    }

    struct paging4gbChunk * chunk4gb = (struct paging4gbChunk *)kzalloc(sizeof(struct paging4gbChunk));
    if(chunk4gb == NULL) goto error;
    chunk4gb->directoryEntry = directory;

    return chunk4gb;
error:
    return NULL;
}


void pagingSwitch(uint32_t * directory)
{
    pagingLoadDirectory(directory);
    currentDirectory = directory;
}


uint32_t* paging4gbChunkGetDirectory(struct paging4gbChunk* chunk)
{
    return chunk->directoryEntry;
} 


bool pagingIsAligned(void* addr)
{
    return ((uint32_t)addr % PAGING_PAGE_SIZE) == 0;
} 



int pagingGetIndexes(void* virtualAddress, uint32_t* directoryIndexOut, uint32_t* tableIndexOut)
{
   int res = 0;
    if (!pagingIsAligned(virtualAddress))
    {
        res = -EINVARG;
        goto out;
    }  

    *directoryIndexOut = ((uint32_t)virtualAddress / (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE));
    *tableIndexOut = ((uint32_t) virtualAddress % (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE) / PAGING_PAGE_SIZE);
out:
    return res;
}


int pagingSet(uint32_t* directory, void* virt, uint32_t val)
{
    if (!pagingIsAligned(virt))
    {
        return -EINVARG;
    }
    uint32_t directoryIndex = 0;
    uint32_t tableIndex = 0;
    int res = pagingGetIndexes(virt, &directoryIndex, &tableIndex);
    if (res < 0)
    {
        return res;
    }
    uint32_t entry = directory[directoryIndex];
    uint32_t* table = (uint32_t*)(entry & 0xfffff000);
    table[tableIndex] = val;
    return 0;
}