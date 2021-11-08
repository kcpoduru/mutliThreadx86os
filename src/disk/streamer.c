#include "streamer.h"


struct diskStream* diskStreamerNew(int diskId)
{
    struct disk* disk = diskGet(diskId);
    if (!disk)
    {
        return 0;
    }

    struct diskStream* streamer = kzalloc(sizeof(struct diskStream));
    streamer->pos = 0;
    streamer->disk = disk;
    return streamer;
}

int diskStreamerSeek(struct diskStream* stream, int pos)
{

    stream->pos = pos;
    return 0;
}

int diskStreamerRead(struct diskStream* stream, void* out, int total)
{
    int sector = stream->pos / PKOS_SECTOR_SIZE;
    int offset = stream->pos % PKOS_SECTOR_SIZE;
    char buf[PKOS_SECTOR_SIZE];

    int res = diskReadBlock(stream->disk, sector, 1, buf);
    if (res < 0)
    {
        goto out;
    }

    int totalToRead = total > PKOS_SECTOR_SIZE ? PKOS_SECTOR_SIZE : total;
    for (int i = 0; i < totalToRead; i++)
    {
        *(char*)out++ = buf[offset+i];
    }

    // Adjust the stream
    stream->pos += totalToRead;
    if (total > PKOS_SECTOR_SIZE)
    {
        res = diskStreamerRead(stream, out, total-PKOS_SECTOR_SIZE);
    }
out:
    return res;
}


void diskStreamerClose(struct diskStream* stream)
{
    kfree(stream);
}
