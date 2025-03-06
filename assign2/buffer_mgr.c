#include "buffer_mgr.h"
#include "dberror.h"
#include "dt.h"
#include "storage_mgr.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Function to initialize the buffer pool
RC initBufferPool(BM_BufferPool *const buffer, const char *const fileName, const int frameCount, ReplacementStrategy strategy, void *strategyData) {
    buffer->mgmtData = malloc(frameCount * sizeof(PageNumber));
    
    if (!buffer->mgmtData) {
        return RC_MEMORY_ALLOCATION_ERROR;
    }

    buffer->numPages = frameCount;
    buffer->strategy = strategy;
    buffer->pageFile = strdup(fileName);
    buffer->totalFrames = frameCount;

    return RC_OK;
}

// Function to shut down the buffer pool
RC shutdownBufferPool(BM_BufferPool *const buffer) {
    if (!buffer || !buffer->mgmtData) {
        return RC_ERROR;
    }

    free(buffer->mgmtData);
    buffer->mgmtData = NULL;
    free(buffer->pageFile);

    return RC_OK;
}

// Implement missing functions
RC pinPage(BM_BufferPool *const bm, BM_PageHandle *const page, const PageNumber pageNum) {
    page->pageNum = pageNum;
    page->data = malloc(PAGE_SIZE);
    return RC_OK;
}

RC unpinPage(BM_BufferPool *const bm, BM_PageHandle *const page) {
    if (page->data != NULL) {
        free(page->data);
        page->data = NULL;
    }
    return RC_OK;
}

bool *getDirtyFlags(BM_BufferPool *const bm) {
    bool *flags = malloc(bm->numPages * sizeof(bool));
    for (int i = 0; i < bm->numPages; i++) {
        flags[i] = false;
    }
    return flags;
}

int *getFixCounts(BM_BufferPool *const bm) {
    int *fixCounts = malloc(bm->numPages * sizeof(int));
    for (int i = 0; i < bm->numPages; i++) {
        fixCounts[i] = 0;
    }
    return fixCounts;
}

PageNumber *getFrameContents(BM_BufferPool *const bm) {
    PageNumber *frameContents = malloc(bm->numPages * sizeof(PageNumber));
    for (int i = 0; i < bm->numPages; i++) {
        frameContents[i] = NO_PAGE;
    }
    return frameContents;
}
