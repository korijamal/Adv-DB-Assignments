#ifndef DBERROR_H
#define DBERROR_H

#include <stdio.h>

#define PAGE_SIZE 4096

typedef int RC;

#define RC_OK 0
#define RC_FILE_NOT_FOUND 1
#define RC_WRITE_FAILED 3
#define RC_READ_FAILED 4
#define RC_ERROR 5
#define RC_FILE_CREATION_FAILED 6
#define RC_READ_NON_EXISTING_PAGE 7
#define RC_MEMORY_ALLOCATION_ERROR 8  

extern char *RC_message;
void printError(RC error);

#endif
