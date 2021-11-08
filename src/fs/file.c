#include "file.h"

struct filesystem* filesystems[PKOS_MAX_FILESYSTEMS];
struct fileDescriptor* fileDescriptors[PKOS_MAX_FILE_DESCRIPTORS];


static struct filesystem** fsGetFreeFilesystem()
{
    int i = 0;
    for (i = 0; i < PKOS_MAX_FILESYSTEMS; i++)
    {
        if (filesystems[i] == 0)
        {
            return &filesystems[i];
        }
    }

    return 0;
}


void fsInsertFilesystem(struct filesystem *filesystem)
{
    struct filesystem** fs;
    fs = fsGetFreeFilesystem();
    if (!fs)
    {
        print("Problem inserting filesystem"); 
        while(1) {}
    }

    *fs = filesystem;
}


static void fsStaticLoad()
{
    //fsInsertFilesystem(fat16_init());
}

void fsLoad()
{
    memset(filesystems, 0, sizeof(filesystems));
    fsStaticLoad();
}




void fsInit()
{
    memset(fileDescriptors, 0, sizeof(fileDescriptors));
    fsLoad();
}


static int fileNewDescriptor(struct fileDescriptor** descOut)
{
    int res = -ENOMEM;
    for (int i = 0; i < PKOS_MAX_FILE_DESCRIPTORS; i++)
    {
        if (fileDescriptors[i] == 0)
        {
            struct fileDescriptor* desc = kzalloc(sizeof(struct fileDescriptor));
            // Descriptors start at 1
            desc->index = i + 1;
            fileDescriptors[i] = desc;
            *descOut = desc;
            res = 0;
            break;
        }
    }

    return res;
}


int fopen(const char* filename, const char* mode)
{
    return -EIO;
}



struct filesystem* fsResolve(struct disk* disk)
{
    struct filesystem* fs = 0;
    for (int i = 0; i < PKOS_MAX_FILESYSTEMS; i++)
    {
        if (filesystems[i] != 0 && filesystems[i]->resolve(disk) == 0)
        {
            fs = filesystems[i];
            break;
        }
    }

    return fs;
}