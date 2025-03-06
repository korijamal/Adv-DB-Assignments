#include "storage_mgr.h"
#include "dberror.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialize Storage Manager
void initStorageManager(void) {
    printf("Storage Manager Initialized\n");
}

// Create a new page file
RC createPageFile(char *fileName) {
    FILE *file = fopen(fileName, "w");

    if (file == NULL) {
        return RC_FILE_NOT_FOUND + RC_FILE_CREATION_FAILED;
    }

    char buffer[PAGE_SIZE];
    memset(buffer, 0, PAGE_SIZE);  

    fwrite(buffer, sizeof(char), PAGE_SIZE, file);
    fclose(file);

    return RC_OK;
}

// Delete page file
RC destroyPageFile(char *fileName) {
    if (remove(fileName) == 0) {
        return RC_OK;
    } else {
        return RC_FILE_NOT_FOUND;
    }
}
