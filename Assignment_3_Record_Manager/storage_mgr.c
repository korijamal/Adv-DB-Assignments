#include "stdio.h"
#include "storage_mgr.h"
#include "dberror.h"
#include "test_helper.h"
#include <stdbool.h>

// Initializing variables
FILE* filePointer;

// Function to initialize the storage manager
void initStorageManager() {
    filePointer = NULL;
}

// Function to create a new page file
RC createPageFile(char *fileName) {
    filePointer = fopen(fileName, "w+");

    if (filePointer == NULL) {
        RC_message = "File creation failed";
        return RC_FILE_NOT_FOUND + RC_FILE_CREATION_FAILED;
    }

    if (filePointer != NULL) {
        char buffer[PAGE_SIZE];
        memset(buffer, 0, PAGE_SIZE);
        fwrite(buffer, sizeof(char), PAGE_SIZE, filePointer);
        fclose(filePointer);
    }
    RC_message = "File created successfully";

    return (filePointer == NULL) ? RC_FILE_NOT_FOUND + RC_FILE_CREATION_FAILED : RC_OK;
}

// Function to open an existing page file
RC openPageFile(char *fileName, SM_FileHandle *fileHandle) {
    filePointer = fopen(fileName, "r+");

    if (filePointer == NULL) {
        RC_message = "File not found";
        return RC_FILE_NOT_FOUND;
    } else {
        fileHandle->fileName = fileName;
        fileHandle->totalNumPages = 0;
        fileHandle->mgmtInfo = filePointer;

        fseek(filePointer, 0, SEEK_END);
        fileHandle->totalNumPages = (int)(ftell(filePointer) / PAGE_SIZE);
        fseek(filePointer, 0, SEEK_SET);

        fileHandle->curPagePos = 0;

        RC_message = "File opened successfully";
        return RC_OK;
    }
}

// Function to close a page file
RC closePageFile(SM_FileHandle *fileHandle) {
    if (fileHandle->mgmtInfo == NULL) {
        RC_message = "File not found";
        return RC_FILE_NOT_FOUND;
    }

    fclose(fileHandle->mgmtInfo);
    fileHandle->mgmtInfo = NULL;
    RC_message = "File closed successfully";

    return RC_OK;
}

// Function to destroy a page file
RC destroyPageFile(char *fileName) {
    int status = remove(fileName);

    if (status == 0) {
        RC_message = "File destroyed successfully.";
        return RC_OK;
    } else {
        RC_message = "File not found.";
        return RC_FILE_NOT_FOUND;
    }
}

// Function to read a block from a page file
RC readBlock(int pageNum, SM_FileHandle *fileHandle, SM_PageHandle memPage) {
    if (fileHandle->mgmtInfo == NULL) {
        RC_message = "File not found.";
        return RC_FILE_NOT_FOUND + RC_READ_NON_EXISTING_PAGE + RC_READ_FAILED;
    } else if (pageNum < 0 || pageNum >= fileHandle->totalNumPages) {
        RC_message = "Invalid page number.";
        return RC_FILE_NOT_FOUND + RC_READ_NON_EXISTING_PAGE;
    } else {
        fseek(fileHandle->mgmtInfo, pageNum * PAGE_SIZE, SEEK_SET);
        fread(memPage, sizeof(char), PAGE_SIZE, fileHandle->mgmtInfo);
        fileHandle->curPagePos = pageNum;
        RC_message = "File read successfully.";
        return RC_OK;
    }
}

// Function to compare two strings
bool stringsMatch(const char *str1, const char *str2) {
    return strcmp(str1, str2) == 0;
}

// Function to get the page position based on the type
int getPagePosition(SM_FileHandle *fileHandle, char *type) {
    switch (0) {
        case 0:
            if (stringsMatch(type, "end_page")) goto endPage;
            if (stringsMatch(type, "preceding_page")) goto precedingPage;
            if (stringsMatch(type, "present_page")) goto presentPage;
            if (stringsMatch(type, "succeeding_page")) goto succeedingPage;
            goto defaultCase;
        endPage:
            return fileHandle->totalNumPages - 1;
        precedingPage:
            return fileHandle->curPagePos - 1;
        presentPage:
            return fileHandle->curPagePos;
        succeedingPage:
            return fileHandle->curPagePos + 1;
        defaultCase:
            return 0;
    }
}

// Function to get the current block position
int getBlockPosition(SM_FileHandle *fileHandle) {
    return fileHandle->curPagePos;
}

// Function to read the first block of a page file
RC readFirstBlock(SM_FileHandle *fileHandle, SM_PageHandle memPage) {
    int pagePos = getPagePosition(fileHandle, "first_page");
    return readBlock(pagePos, fileHandle, memPage);
}

// Function to read the last block of a page file
RC readLastBlock(SM_FileHandle *fileHandle, SM_PageHandle memPage) {
    return readBlock(getPagePosition(fileHandle, "end_page"), fileHandle, memPage);
}

// Function to read the previous block of the current block in a page file
RC readPreviousBlock(SM_FileHandle *fileHandle, SM_PageHandle memPage) {
    return readBlock(getPagePosition(fileHandle, "preceding_page"), fileHandle, memPage);
}

// Function to read the current block in a page file
RC readCurrentBlock(SM_FileHandle *fileHandle, SM_PageHandle memPage) {
    return readBlock(getPagePosition(fileHandle, "present_page"), fileHandle, memPage);
}

// Function to read the next block of the current block in a page file
RC readNextBlock(SM_FileHandle *fileHandle, SM_PageHandle memPage) {
    return readBlock(getPagePosition(fileHandle, "succeeding_page"), fileHandle, memPage);
}

// Function to write a block to a page file
RC writeBlock(int pageNum, SM_FileHandle *fileHandle, SM_PageHandle memPage) {
    if (fileHandle->mgmtInfo == NULL) {
        RC_message = "File not found";
        return RC_FILE_NOT_FOUND;
    } else if (pageNum < 0 || pageNum > fileHandle->totalNumPages) {
        RC_message = "Requested page not available";
        return RC_READ_NON_EXISTING_PAGE;
    } else {
        fseek(fileHandle->mgmtInfo, pageNum * PAGE_SIZE, SEEK_SET);
        size_t writtenBytes = 0;
        size_t bytesCount = 0;
        for (; writtenBytes < PAGE_SIZE; writtenBytes += bytesCount) {
            bytesCount = fwrite(memPage + writtenBytes, sizeof(char), PAGE_SIZE - writtenBytes, fileHandle->mgmtInfo);
            if (bytesCount == 0) {
                return RC_WRITE_FAILED;
            }
        }
        fileHandle->curPagePos = pageNum;
        fclose(fileHandle->mgmtInfo);
        RC_message = "Data successfully updated into the file";
    }
    return RC_OK;
}

// Function to write to the current block in a page file
RC writeCurrentBlock(SM_FileHandle *fileHandle, SM_PageHandle memPage) {
    return writeBlock(getPagePosition(fileHandle, "present_page"), fileHandle, memPage);
}

// Function to append an empty block to a page file
RC appendEmptyBlock(SM_FileHandle *fileHandle) {
    if (fileHandle->mgmtInfo == NULL) {
        RC_message = "File not found";
        return RC_FILE_NOT_FOUND;
    } else {
        fseek(fileHandle->mgmtInfo, 0, SEEK_END);
        char emptyBuffer[PAGE_SIZE] = {'\0'};

        fwrite(emptyBuffer, sizeof(char), PAGE_SIZE, fileHandle->mgmtInfo);

        int numPages = fileHandle->totalNumPages + 1;
        fileHandle->totalNumPages = numPages;

        int currentPage = fileHandle->totalNumPages - 1;
        fileHandle->curPagePos = currentPage;

        RC_message = "File append completed: empty block appended and filled with null values";
        return RC_OK;
    }
}

// Function to ensure the capacity of a page file
RC ensureCapacity(int numberOfPages, SM_FileHandle *fileHandle) {
    if (fileHandle->mgmtInfo == NULL) {
        RC_message = "File not found.";
        return RC_FILE_NOT_FOUND;
    } else {
        int totalPages = fileHandle->totalNumPages;
        while (numberOfPages > totalPages) {
            appendEmptyBlock(fileHandle);
            totalPages = fileHandle->totalNumPages;
        }
        return RC_OK;
    }
}