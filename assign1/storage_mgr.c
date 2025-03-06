#include "storage_mgr.h"
#include "dberror.h"
#include <stdlib.h>
#include <string.h>

// Initialize storage manager
void initStorageManager(void) {
    printf("Storage Manager Initialized.\n");
}

// Create a new page file
RC createPageFile(char *fileName) {
    FILE *file = fopen(fileName, "wb+");
    if (!file) return RC_FILE_NOT_FOUND;

    char emptyPage[PAGE_SIZE] = {0};
    fwrite(emptyPage, sizeof(char), PAGE_SIZE, file);
    fclose(file);
    return RC_OK;
}

// Open an existing page file
RC openPageFile(char *fileName, SM_FileHandle *fHandle) {
    FILE *file = fopen(fileName, "rb+");
    if (!file) return RC_FILE_NOT_FOUND;

    fseek(file, 0, SEEK_END);
    int totalPages = ftell(file) / PAGE_SIZE;

    fHandle->fileName = strdup(fileName);
    fHandle->totalNumPages = totalPages;
    fHandle->curPagePos = 0;
    fHandle->mgmtInfo = file;

    return RC_OK;
}

// Close a file
RC closePageFile(SM_FileHandle *fHandle) {
    fclose((FILE *)fHandle->mgmtInfo);
    fHandle->mgmtInfo = NULL;
    return RC_OK;
}

// Delete a file
RC destroyPageFile(char *fileName) {
    if (remove(fileName) != 0) return RC_FILE_NOT_FOUND;
    return RC_OK;
}

// Read block
RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
    fseek((FILE *)fHandle->mgmtInfo, pageNum * PAGE_SIZE, SEEK_SET);
    fread(memPage, sizeof(char), PAGE_SIZE, (FILE *)fHandle->mgmtInfo);
    fHandle->curPagePos = pageNum;
    return RC_OK;
}

// Read first block
RC readFirstBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return readBlock(0, fHandle, memPage);
}

// Read previous block
RC readPreviousBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return readBlock(fHandle->curPagePos - 1, fHandle, memPage);
}

// Read current block
RC readCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return readBlock(fHandle->curPagePos, fHandle, memPage);
}

// Read next block
RC readNextBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return readBlock(fHandle->curPagePos + 1, fHandle, memPage);
}

// Read last block
RC readLastBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return readBlock(fHandle->totalNumPages - 1, fHandle, memPage);
}

// Write a block
RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
    if (!fHandle->mgmtInfo) return RC_FILE_HANDLE_NOT_INIT;
    FILE *file = (FILE *)fHandle->mgmtInfo;
    
    if (pageNum < 0 || pageNum >= fHandle->totalNumPages) return RC_READ_NON_EXISTING_PAGE;
    
    fseek(file, pageNum * PAGE_SIZE, SEEK_SET);
    fwrite(memPage, sizeof(char), PAGE_SIZE, file);
    fHandle->curPagePos = pageNum;
    return RC_OK;
}

// Write to current block
RC writeCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) {
    return writeBlock(fHandle->curPagePos, fHandle, memPage);
}

// Append an empty block
RC appendEmptyBlock(SM_FileHandle *fHandle) {
    FILE *file = (FILE *)fHandle->mgmtInfo;
    char emptyPage[PAGE_SIZE] = {0};
    fseek(file, 0, SEEK_END);
    fwrite(emptyPage, sizeof(char), PAGE_SIZE, file);
    fHandle->totalNumPages++;
    return RC_OK;
}

// Ensure file has at least `n` pages
RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle) {
    while (fHandle->totalNumPages < numberOfPages) {
        appendEmptyBlock(fHandle);
    }
    return RC_OK;
}
