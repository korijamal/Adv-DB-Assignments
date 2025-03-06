#include "stdio.h"
#include "storage_mgr.h"
#include "dberror.h"
#include "test_helper.h"
#include <stdbool.h>


// Initializing variables
FILE* fptr_17;

// Function to initialize the storage manager
void initStorageManager()
{
    fptr_17 = NULL;
}

// Function to create a new page file
RC createPageFile(char *fileName)
{
    fptr_17 = fopen(fileName, "w+");

    if (fptr_17 == NULL)
    {
        RC_message = "File Was Not Created";
        return RC_FILE_NOT_FOUND + RC_FILE_CREATION_FAILED ;
    }

    if (fptr_17 != NULL)
    {
        char buffer[PAGE_SIZE];
        memset(buffer, 0, PAGE_SIZE); 
        fwrite(buffer, sizeof(char), PAGE_SIZE, fptr_17);
        fclose(fptr_17);
    }
    RC_message = "File Created Successfully";

    return (fptr_17 == NULL) ? RC_FILE_NOT_FOUND + RC_FILE_CREATION_FAILED : RC_OK;
}

// Function to open an existing page file
RC openPageFile(char *fileName, SM_FileHandle *fHandle)
{
    fptr_17 = fopen(fileName, "r+");

    if(fptr_17 == NULL)
    {
        RC_message = "File Was Not Found";
        return RC_FILE_NOT_FOUND;
    }
    else
    {
        fHandle->fileName = fileName;
        fHandle->totalNumPages = 0;
        fHandle->mgmtInfo = fptr_17;

        fseek(fptr_17,0 ,SEEK_END);
        fHandle->totalNumPages = (int)(ftell(fptr_17) / PAGE_SIZE);
        fseek(fptr_17,0,SEEK_SET);

        fHandle->curPagePos = 0;

        RC_message = "File Opened Successfully";
        return RC_OK;
    }
}

// Function to close a page file
RC closePageFile(SM_FileHandle *fHandle)
{
    if (fHandle->mgmtInfo == NULL) {
        RC_message = "File Was Not Found";
        return RC_FILE_NOT_FOUND;
    }

    fclose(fHandle->mgmtInfo);
    fHandle->mgmtInfo = NULL;
    RC_message = "File Closed Successfully";
    
    return RC_OK;
}

// Function to destroy a page file
RC destroyPageFile(char *fileName)
{
    int clear_page_file = remove(fileName);

    if (clear_page_file == 0) {
        RC_message = "File Destroyed Successfully.";
        return RC_OK;
    } 
    else {
        RC_message = "File Was Not Found.";
        return RC_FILE_NOT_FOUND;
    }
}

// Function to read a block from a page file
RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
    // Check if the file management info is null or the page number is out of valid range
    if (fHandle->mgmtInfo == NULL) {
        RC_message = "File Not Found.";
        return RC_FILE_NOT_FOUND + RC_READ_NON_EXISTING_PAGE + RC_READ_FAILED ;
    } 
    else if (pageNum < 0 || pageNum >= fHandle->totalNumPages) {
        RC_message = "Invalid page number.";
        return RC_FILE_NOT_FOUND + RC_READ_NON_EXISTING_PAGE;
    } else {
        // Seek to the correct page position
        fseek(fHandle->mgmtInfo, pageNum * PAGE_SIZE, SEEK_SET);

        // Read the specified page into memory
        fread(memPage, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo);

        // Update the current page position in the file handle
        fHandle->curPagePos = pageNum;

        // Set the success message
        RC_message = "File Read Successfully.";
        return RC_OK;
    }
}

// Function to compare two strings
bool strings_match(const char *str1, const char *str2) {
    return strcmp(str1, str2) == 0;
}

// Function to get the page position based on the type
int Get_Page_Pos(SM_FileHandle *fHandle, char *type) {
    switch (0) { // Using switch with a constant for string comparisons
        case 0:
            if (strings_match(type, "end_page")) goto end_page;
            if (strings_match(type, "preceding_page")) goto preceding_page;
            if (strings_match(type, "present_page")) goto present_page;
            if (strings_match(type, "succeeding_page")) goto succeeding_page;
            goto default_case;
        end_page:
            return fHandle->totalNumPages - 1;
        preceding_page:
            return fHandle->curPagePos - 1;
        present_page:
            return fHandle->curPagePos;
        succeeding_page:
            return fHandle->curPagePos + 1;
        default_case:
            return 0;
    }
}

// Function to get the current block position
int getBlockPos(SM_FileHandle *fHandle)
{
    return fHandle->curPagePos;
}

// Function to read the first block of a page file
RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    int pagePos = Get_Page_Pos(fHandle, "first_page");
    return readBlock(pagePos, fHandle, memPage);
}

// Function to read the last block of a page file
RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    return readBlock(Get_Page_Pos(fHandle, "end_page"),fHandle,memPage);
}

// Function to read the previous block of the current block in a page file
RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    return readBlock(Get_Page_Pos(fHandle, "preceding_page"),fHandle,memPage);
}

// Function to read the current block in a page file
RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    return readBlock(Get_Page_Pos(fHandle, "present_page"),fHandle,memPage);
}

// Function to read the next block of the current block in a page file
RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    return readBlock(Get_Page_Pos(fHandle, "succeeding_page"),fHandle,memPage);
}   

// Function to write a block to a page file
RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if (fHandle->mgmtInfo == NULL)
    {
        RC_message = "File Was Not Found";
        return RC_FILE_NOT_FOUND;
    }
    else if (pageNum < 0 || pageNum > fHandle->totalNumPages)
    {
        RC_message = "Requested Page Not Available";
        return RC_READ_NON_EXISTING_PAGE;
    }
    else
    {
        fseek(fHandle->mgmtInfo, pageNum * PAGE_SIZE, SEEK_SET);
        size_t writtenBytes = 0;
        size_t bytes_count = 0;  // Initialize to 0
        for (; writtenBytes < PAGE_SIZE; writtenBytes += bytes_count) {
            bytes_count = fwrite(memPage + writtenBytes, sizeof(char), PAGE_SIZE - writtenBytes, fHandle->mgmtInfo);
            if (bytes_count == 0) {
                return RC_WRITE_FAILED;
            }
        }
        fHandle->curPagePos = pageNum;
        fclose(fHandle->mgmtInfo);
        RC_message = "Data successfully updated into the file";
    }
    return RC_OK;
}


// Function to write to the current block in a page file
RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	return writeBlock(Get_Page_Pos(fHandle, "present_page"),fHandle,memPage);
}

// Function to append an empty block to a page file
RC appendEmptyBlock (SM_FileHandle *fHandle)
{
    if (fHandle->mgmtInfo == NULL)
    {
        RC_message = "File Was Not Found";
        return RC_FILE_NOT_FOUND;
    }
    else
    {
        fseek(fHandle->mgmtInfo, 0, SEEK_END);
        char empty_buffer[PAGE_SIZE] = {'\0'};

        fwrite(empty_buffer, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo);

        int num_pages = fHandle->totalNumPages + 1;
        fHandle->totalNumPages = num_pages;

        int current_page = fHandle->totalNumPages - 1;
        fHandle->curPagePos = current_page;

        RC_message = "File Append completed: empty block appended and filled with null values";
        return RC_OK;
    }
}

// Function to ensure the capacity of a page file
RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
{
	if(fHandle->mgmtInfo == NULL)
    {
        RC_message = "File Was Not Found.";
        return RC_FILE_NOT_FOUND;
    }
    else
    {
        int total_pages = fHandle->totalNumPages;
        while(numberOfPages > total_pages)
        {
            appendEmptyBlock(fHandle);
            total_pages = fHandle->totalNumPages;
        }
        return RC_OK;
    }
}