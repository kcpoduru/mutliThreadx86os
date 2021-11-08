#ifndef DISKSTREAMER_H
#define DISKSTREAMER_H

#include "disk.h"
#include "memory/heap/kheap.h"
#include "config.h"

struct diskStream
{
    int pos;
    struct disk* disk;
};

struct diskStream* diskStreamerNew(int diskId);
int diskStreamerSeek(struct diskStream* stream, int pos);
int diskStreamerRead(struct diskStream* stream, void* out, int total);
void diskStreamerClose(struct diskStream* stream);


#endif