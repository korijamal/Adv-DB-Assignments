#ifndef BUFFER_MGR_H
#define BUFFER_MGR_H

#include "dberror.h"
#include "dt.h"

#define NO_PAGE -1  
// Page Replacement Strategies
typedef enum ReplacementStrategy {
    RS_FIFO = 0,
    RS_LRU = 1,
    RS_CLOCK = 2,
    RS_LFU = 3,
    RS_LRU_K = 4
} ReplacementStrategy;

// Buffer Pool Structure
typedef struct BM_BufferPool {
    char *pageFile;
    int numPages;
    ReplacementStrategy strategy;
    void *mgmtData;
    int totalFrames;  
} BM_BufferPool;

// Page Handle Structure
typedef struct BM_PageHandle {
    PageNumber pageNum;
    char *data;
} BM_PageHandle;

// Function Prototypes
RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName, const int numPages, ReplacementStrategy strategy, void *stratData);
RC shutdownBufferPool(BM_BufferPool *const bm);
RC forceFlushPool(BM_BufferPool *const bm);
RC markDirty(BM_BufferPool *const bm, BM_PageHandle *const page);
RC unpinPage(BM_BufferPool *const bm, BM_PageHandle *const page);
RC forcePage(BM_BufferPool *const bm, BM_PageHandle *const page);
RC pinPage(BM_BufferPool *const bm, BM_PageHandle *const page, const PageNumber pageNum);

PageNumber *getFrameContents(BM_BufferPool *const bm);
bool *getDirtyFlags(BM_BufferPool *const bm);
int *getFixCounts(BM_BufferPool *const bm);
int getNumReadIO(BM_BufferPool *const bm);
int getNumWriteIO(BM_BufferPool *const bm);

#endif
