#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define PAGING_CACHE_DISABLED  0b00010000
#define PAGING_WRITE_THROUGH   0b00001000
#define PAGING_ACCESS_FROM_ALL 0b00000100
#define PAGING_IS_WRITEABLE    0b00000010
#define PAGING_IS_PRESENT      0b00000001

#define PAGING_TOTAL_ENTRIES_PER_TABLE 1024
#define PAGING_PAGE_SIZE 4096



struct paging4gbChunk
{
    uint32_t* directoryEntry;
};

struct paging4gbChunk* pagingNew4gb(uint8_t flags);
void pagingSwitch(uint32_t* directory);
void enablePaging();
int pagingSet(uint32_t* directory, void* virt, uint32_t val);
int pagingGetIndexes(void* virtualAddress, uint32_t* directoryIndexOut, uint32_t* tableIndexOut);
bool pagingIsAligned(void* addr);
uint32_t* paging4gbChunkGetDirectory(struct paging4gbChunk* chunk);
#endif 