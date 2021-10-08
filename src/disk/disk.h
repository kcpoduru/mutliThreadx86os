#ifndef DISK_H
#define DISK_H

typedef unsigned int PEACHOS_DISK_TYPE;

#define PEACHOS_DISK_TYPE_REAL                    0


struct disk
{
    PEACHOS_DISK_TYPE type;
    int sectorSize;
};

void diskSearchAndInit();
struct disk* diskGet(int index);
int diskReadBlock(struct disk* idisk, unsigned int lba, int total, void* buf);


#endif 