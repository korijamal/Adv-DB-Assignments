#include "storage_mgr.h"
#include "buffer_mgr_stat.h"
#include "buffer_mgr.h"
#include "dberror.h"
#include "test_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void testLRU() {
    printf("Starting LRU test...\n");

    BM_BufferPool *bm = malloc(sizeof(BM_BufferPool));
    BM_PageHandle *h = malloc(sizeof(BM_PageHandle));

    if (bm == NULL || h == NULL) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    printf("Creating page file...\n");
    CHECK(createPageFile("testfile.bin"));
    
    printf("Initializing buffer pool with LRU strategy...\n");
    CHECK(initBufferPool(bm, "testfile.bin", 3, RS_LRU, NULL));

    printf("Pinning page 0...\n");
    CHECK(pinPage(bm, h, 0));

    printf("Unpinning page 0...\n");
    CHECK(unpinPage(bm, h));

    printf("Shutting down buffer pool...\n");
    CHECK(shutdownBufferPool(bm));

    printf("Destroying page file...\n");
    CHECK(destroyPageFile("testfile.bin"));

    free(bm);
    free(h);

    printf("LRU test completed successfully!\n");
}

int main(void) {
    testLRU();
    return 0;
}
