#ifndef STORAGE_MGR_H
#define STORAGE_MGR_H

#include "dberror.h"

typedef struct SM_FileHandle {
    char *fileName;
    int totalNumPages;
    int curPagePos;
    void *mgmtInfo;
} SM_FileHandle;

typedef char* SM_PageHandle;

void initStorageManager(void);
RC createPageFile(char *fileName);
RC openPageFile(char *fileName, SM_FileHandle *fHandle);
RC closePageFile(SM_FileHandle *fHandle);
RC destroyPageFile(char *fileName);

#endif
