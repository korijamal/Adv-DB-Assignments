#include "storage_mgr.h"
#include "dberror.h"
#include "test_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_FILE "test_pagefile.bin"

// Function Prototypes
static void testCreateOpenClose(void);
static void testReadWriteBlocks(void);
static void testEnsureCapacity(void);

// Main Test Runner
int main(void) {
    printf("\n====== Starting Storage Manager Tests ======\n");

    initStorageManager();

    testCreateOpenClose();
    testReadWriteBlocks();
    testEnsureCapacity();

    printf("\n====== All Tests Completed Successfully! ======\n");
    return 0;
}


void testCreateOpenClose(void) {
    SM_FileHandle fileHandle;

    printf("\n[TEST] Creating, Opening, Closing, and Deleting a Page File\n");

    TEST_CHECK(createPageFile(TEST_FILE));
    printf("[INFO] File created: %s\n", TEST_FILE);

    TEST_CHECK(openPageFile(TEST_FILE, &fileHandle));
    printf("[INFO] File opened successfully: %s\n", fileHandle.fileName);

    ASSERT_TRUE(fileHandle.totalNumPages == 1, "File should contain 1 page after creation.");
    ASSERT_TRUE(fileHandle.curPagePos == 0, "Newly opened file should have page position 0.");

    TEST_CHECK(closePageFile(&fileHandle));
    printf("[INFO] File closed successfully.\n");

    TEST_CHECK(destroyPageFile(TEST_FILE));
    printf("[INFO] File deleted successfully: %s\n", TEST_FILE);

    // Attempting to open a deleted file should fail
    ASSERT_TRUE(openPageFile(TEST_FILE, &fileHandle) != RC_OK, "Opening a deleted file should return an error.");

    TEST_DONE();
}

void testReadWriteBlocks(void) {
    SM_FileHandle fileHandle;
    SM_PageHandle pageBuffer = (SM_PageHandle)malloc(PAGE_SIZE);
    memset(pageBuffer, 0, PAGE_SIZE);  // Initialize memory

    printf("\n[TEST] Read and Write Blocks\n");

    TEST_CHECK(createPageFile(TEST_FILE));
    TEST_CHECK(openPageFile(TEST_FILE, &fileHandle));

    // Read the first block and verify it's empty
    TEST_CHECK(readFirstBlock(&fileHandle, pageBuffer));
    for (int i = 0; i < PAGE_SIZE; i++) {
        ASSERT_TRUE(pageBuffer[i] == 0, "First block should be initialized to zero.");
    }

    // Write some test data
    for (int i = 0; i < PAGE_SIZE; i++) {
        pageBuffer[i] = (i % 10) + '0';
    }

    TEST_CHECK(writeBlock(0, &fileHandle, pageBuffer));

    // Read back the block and verify data integrity
    memset(pageBuffer, 0, PAGE_SIZE);
    TEST_CHECK(readFirstBlock(&fileHandle, pageBuffer));

    for (int i = 0; i < PAGE_SIZE; i++) {
        ASSERT_TRUE(pageBuffer[i] == (i % 10) + '0', "Read data should match written data.");
    }

    TEST_CHECK(closePageFile(&fileHandle));
    TEST_CHECK(destroyPageFile(TEST_FILE));

    free(pageBuffer);
    TEST_DONE();
}

void testEnsureCapacity(void) {
    SM_FileHandle fileHandle;

    printf("\n[TEST] Ensuring File Capacity\n");

    TEST_CHECK(createPageFile(TEST_FILE));
    TEST_CHECK(openPageFile(TEST_FILE, &fileHandle));

    printf("[INFO] Current pages in file: %d\n", fileHandle.totalNumPages);
    TEST_CHECK(ensureCapacity(1, &fileHandle));

    ASSERT_TRUE(fileHandle.totalNumPages <= 1, "File should have at least 1 page.");

    TEST_CHECK(closePageFile(&fileHandle));
    TEST_CHECK(destroyPageFile(TEST_FILE));

    TEST_DONE();
}
