#include "storage_mgr.h"
#include "buffer_mgr_stat.h"
#include "buffer_mgr.h"
#include "dberror.h"
#include "test_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void testLRU() {
    BM_BufferPool *bm = malloc(sizeof(BM_BufferPool));
    BM_PageHandle *h = malloc(sizeof(BM_PageHandle));

    CHECK(createPageFile("testfile.bin"));
    CHECK(initBufferPool(bm, "testfile.bin", 3, RS_LRU, NULL));

    CHECK(pinPage(bm, h, 0));
    CHECK(unpinPage(bm, h));

    CHECK(shutdownBufferPool(bm));
    CHECK(destroyPageFile("testfile.bin"));

    free(bm);
    free(h);
}

int main(void) {
    testLRU();
    return 0;
}
