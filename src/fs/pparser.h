#ifndef PATHPARSER_H
#define PATHPARSER_H


struct pathPart
{
    const char* part;
    struct pathPart* next;
};


struct pathRoot
{
    int driveNo;
    struct pathPart* first;
};


struct pathRoot* pathParserParse(const char* path, const char* currentDirectoryPath);
void pathparserFree(struct pathRoot* root);

#endif 