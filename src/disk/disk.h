#ifndef DISK_H
#define DISK_H

#include "fs/file.h"

typedef unsigned int PEACHOS_DISK_TYPE;

#define PEACHOS_DISK_TYPE_REAL                    0


struct disk
{
    PEACHOS_DISK_TYPE type;
    int sectorSize;
    struct filesystem* filesystem;
};

void diskSearchAndInit();
struct disk* diskGet(int index);
int diskReadBlock(struct disk* idisk, unsigned int lba, int total, void* buf);


#endif 