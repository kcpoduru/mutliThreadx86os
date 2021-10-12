#include "pparser.h"
#include "kernel.h"
#include "string/string.h"
#include "memory/heap/kheap.h"
#include "memory/memory.h"
#include "status.h"

static int pathparserPathValidFormat(const char* filename)
{
    int len = strnlen(filename, PKOS_MAX_PATH);
    return (len >= 3 && isdigit(filename[0]) && memcmp((void*)&filename[1], ":/", 2) == 0);
}

static int pathparserGetDriveByPath(const char** path)
{
    if(!pathparserPathValidFormat(*path))
    {
        return -EBADPATH;
    }

    int driveNo = toNumericDigit(*path[0]);

    // Add 3 bytes to skip drive number 0:/ 1:/ 2:/
    *path += 3;
    return driveNo;
}

static struct pathRoot* pathparserCreateRoot(int driveNumber)
{
    struct pathRoot* pathR = kzalloc(sizeof(struct pathRoot));
    pathR->driveNo = driveNumber;
    pathR->first = 0;
    return pathR;
}


static const char* pathparserGetPathPart(const char** path)
{
    char* resultPathPart = kzalloc(PKOS_MAX_PATH);
    int i = 0;
    while(**path != '/' && **path != 0x00)
    {
        resultPathPart[i] = **path;
        *path += 1;
        i++;
    }

    if (**path == '/')
    {
        // Skip the forward slash to avoid problems
        *path += 1;
    }

    if(i == 0)
    {
        kfree(resultPathPart);
        resultPathPart = 0;
    }

    return resultPathPart;
}

struct pathPart* pathparserParsePathPart(struct pathPart* lastPart, const char** path)
{
    const char* pathPartStr = pathparserGetPathPart(path);
    if (!pathPartStr)
    {
        return 0;
    }

    struct pathPart* part = kzalloc(sizeof(struct pathPart));
    part->part = pathPartStr;
    part->next = 0x00;

    if (lastPart)
    {
        lastPart->next = part;
    }

    return part;
}

void pathparserFree(struct pathRoot* root)
{
    struct pathPart* part = root->first;
    while(part)
    {
        struct pathPart* nextPart = part->next;
        kfree((void*) part->part);
        kfree(part);
        part = nextPart;
    }

    kfree(root);
}

struct pathRoot* pathparserParse(const char* path, const char* currentDirectoryPath)
{
    int res = 0;
    const char* tmpPath = path;
    struct pathRoot* pathRoot = 0;

    if (strlen(path) > PKOS_MAX_PATH)
    {
        goto out;
    }

    res = pathparserGetDriveByPath(&tmpPath);
    if (res < 0)
    {
        goto out;
    }

    pathRoot = pathparserCreateRoot(res);
    if (!pathRoot)
    {
        goto out;
    }

    struct pathPart* firstPart = pathparserParsePathPart(NULL, &tmpPath);
    if (!firstPart)
    {
        goto out;
    }

    pathRoot->first = firstPart;
    struct pathPart* part = pathparserParsePathPart(firstPart, &tmpPath);
    while(part)
    {
        part = pathparserParsePathPart(part, &tmpPath);
    }

out:
    return pathRoot;
} 