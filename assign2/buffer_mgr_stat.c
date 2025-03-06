#include "buffer_mgr_stat.h"
#include "buffer_mgr.h"
#include <stdio.h>
#include <stdlib.h>

void printPoolContent(BM_BufferPool *const bm) {
    PageNumber *frameContent = getFrameContents(bm);
    bool *dirtyFlags = getDirtyFlags(bm);
    int *fixCounts = getFixCounts(bm);

    printf("{");
    for (int i = 0; i < bm->numPages; i++) {
        printf("[%d%s%d]", frameContent[i], dirtyFlags[i] ? "x" : " ", fixCounts[i]);
    }
    printf("}\n");

    free(frameContent);
    free(dirtyFlags);
    free(fixCounts);
}
