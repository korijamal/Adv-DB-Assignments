#ifndef STORAGE_MGR_H
#define STORAGE_MGR_H

#include "dberror.h"
#include <stdio.h>

// File Handle Structure

typedef struct SM_FileHandle {
    char *fileName; // Name of the file
    int totalNumPages;  // Total number of pages in the file
    int curPagePos; //Current page position 
    void *mgmtInfo; //Internal file management information
} SM_FileHandle;

typedef char *SM_PageHandle;  

// Storage Manager Function

void initStorageManager(void);

// File Handling
RC createPageFile(char *fileName);
RC openPageFile(char *fileName, SM_FileHandle *fHandle);
RC closePageFile(SM_FileHandle *fHandle);
RC destroyPageFile(char *fileName);

// Reading Blocks
RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);
int getBlockPos(SM_FileHandle *fHandle);
RC readFirstBlock(SM_FileHandle *fHandle, SM_PageHandle memPage);
RC readPreviousBlock(SM_FileHandle *fHandle, SM_PageHandle memPage);
RC readCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage);
RC readNextBlock(SM_FileHandle *fHandle, SM_PageHandle memPage);
RC readLastBlock(SM_FileHandle *fHandle, SM_PageHandle memPage);

// Writing Blocks
RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);
RC writeCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage);
RC appendEmptyBlock(SM_FileHandle *fHandle);
RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle);

#endif



