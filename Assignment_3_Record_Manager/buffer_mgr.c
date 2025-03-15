#include "dberror.h"
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "string.h"
#include "dt.h"
#include "storage_mgr.h"
#include "buffer_mgr.h"
#include "test_helper.h"


typedef struct PF_node {
    int fix_counter;
    SM_PageHandle read_content;
    bool is_dirtyFlag;
    bool is_usedFlag;
    int Frame_Idx; 
    BM_PageHandle* buffer_handle; 
} PF_node;

typedef struct Queue {
    int frameNumber; 
    int pageNumber; 
} Queue;

typedef struct PageFrame_MetaData
{    
void *queue; 
int total_Frames_filled;   
int Frames_count;  
} PageFrame_MetaData; 


int write_Count;
int read_Count;


int k;

int clock_Idx = 0; 

struct PageFrame_MetaData pf_metadata;
RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName, const int numPages, ReplacementStrategy strategy, void *stratData) {
    read_Count = 0;
    write_Count = 0;

    if (access(pageFileName, F_OK) != 0) {
        bm->mgmtData = NULL;
        return RC_FILE_NOT_FOUND;
    }
    PF_node *pf_node = (PF_node *)malloc(numPages * sizeof(PF_node));
    Queue *queue = (Queue *)malloc(numPages * sizeof(Queue));
    
    if (!pf_node || !queue) {
        free(pf_node);
        free(queue);
        pf_node = NULL;
        queue = NULL;
        return RC_MEMORY_ALLOCATION_FAILED; 
    }

    for (int i = 0; i < numPages; i++) {
    // Allocate memory for a new page handle
    BM_PageHandle *buffer_handle = MAKE_PAGE_HANDLE();
    
    // Allocate memory for page frame content
    pf_node[i].read_content = (SM_PageHandle)malloc(PAGE_SIZE);

    // Handle memory allocation failure
    if (pf_node[i].read_content == NULL) {
        // Free already allocated resources
        for (int j = 0; j < i; j++) {
            free(pf_node[j].read_content);
        }
        free(pf_node);
        free(queue);
        return RC_MEMORY_ALLOCATION_FAILED;
    }

    // Initialize buffer handle and page frame attributes
    pf_node[i].buffer_handle = buffer_handle;

    // Set default values for the page handle
    buffer_handle->pageNum = NO_PAGE;     // Indicate that no page is currently assigned
    buffer_handle->data = NULL;            // Initialize data pointer to NULL

    // Set frame-specific attributes
    pf_node[i].Frame_Idx = i;              // Set the index of the frame
    pf_node[i].is_dirtyFlag = false;       // Initialize dirty flag to false
    pf_node[i].fix_counter = 0;            // Initialize fix counter to 0

    // Initialize the page queue with default values
    queue[i].pageNumber = NO_PAGE;         // No page number assigned yet
    queue[i].frameNumber = NO_PAGE;        // No frame number assigned yet
    }

    // Initialize the buffer pool structure
    *bm = (BM_BufferPool) {
        .numPages = numPages,               // Set the number of pages in the pool
        .pageFile = (char *)pageFileName,   // Set the name of the page file
        .strategy = strategy,                // Set the replacement strategy
        .mgmtData = pf_node                  // Assign the management data to the buffer pool
    };

    // Initialize metadata for page frames
    pf_metadata = (PageFrame_MetaData) {
        .Frames_count = numPages,            // Set the total frames count
        .total_Frames_filled = 0,           // Initialize filled frames count to 0
        .queue = queue                       // Assign the initialized queue
    };

    return RC_OK;  // Return success status   
}


//shutting down buffer bool 
RC shutdownBufferPool(BM_BufferPool *const bm)
{
    // Retrieve management data as page frame nodes
    PF_node *frameNodes = (PF_node *)bm->mgmtData;

    // Check if the buffer pool has been properly initialized
    if (!frameNodes) {
    RC_message = "Shutdown failed: buffer pool not initialized.";
    return RC_ERROR;
    }

    bool flushNeeded = false; // Flag to track if any dirty pages need flushing

    // Iterate through all page frames in the buffer pool
    int idx = 0;
   while (idx < bm->numPages)
   {
    PF_node *currentFrame = &frameNodes[idx];

    if (currentFrame->is_dirtyFlag)
    {
        forcePage(bm, currentFrame->buffer_handle);
        flushNeeded = true;
    }

    free(currentFrame->buffer_handle);
    free(currentFrame->read_content);
    idx++;
   }


    // Deallocate the queue and the management data for the buffer pool
    free(pf_metadata.queue);
    free(bm->mgmtData);

    // Reset the buffer pool properties to their initial state
    bm->mgmtData = NULL;
    bm->numPages = 0;
    read_Count = 0;
    write_Count = 0;

    // Set the shutdown status message based on flushing
    RC_message = flushNeeded ? "Shutdown successful; all dirty pages flushed."
                         : "Shutdown successful; no dirty pages present.";


    return RC_OK; // Indicate successful shutdown
}

// Flushing all dirty Pages
RC forceFlushPool(BM_BufferPool *const bm)
{
    // Retrieve the page frame nodes from the buffer pool management data
    PF_node *frames = (PF_node *)bm->mgmtData;
    bool flushSuccessful = true;

    // Use a for loop for iteration
        // Check if buffer pool management data exists first
    if (bm->mgmtData != NULL)
    {
        // Iterate over the frames to check if they need to be flushed
        for (int index = 0; index < bm->numPages && flushSuccessful; index++)
        {
            // If the frame is dirty and has no active fix count, force the page to be written
            bool frameNeedsFlush = (frames[index].is_dirtyFlag && frames[index].fix_counter == 0);
            if (frameNeedsFlush)
            {
                RC returnCode = forcePage(bm, frames[index].buffer_handle);  // Try forcing the page write
                if (returnCode != RC_OK)
                {
                    flushSuccessful = false;  // Set flushSuccessful to false if forcing failed
                    RC_message = "Flush operation unsuccessful.";
                }
            }
        }

        // If all pages have been successfully flushed, set success message
        if (flushSuccessful)
        {
            RC_message = "Flush operation completed successfully.";
        }
    }
    else
    {
        // Handle the case when the buffer pool does not exist
        RC_message = "Flush operation failed: no buffer pool available.";
        return RC_ERROR;
    }

    // Return success code based on flush status
    if (flushSuccessful)
    {
        return RC_OK;
    }
    else
    {
        return RC_ERROR;
    }
  // Return based on success or failure of flush
}


/// Interface Access Pages ///

// Write the current content of the page back to the page file on disk
RC forcePage(BM_BufferPool *const pool, BM_PageHandle *const handle)
{
    // Get the array of page frames from the buffer pool management data
    PF_node *pageFrames = (PF_node *)pool->mgmtData;
    SM_FileHandle handleFile;

    int currentIndex = 0;

    // Further logic for forcing page content to disk will go here
    bool pageExists = false;

    // Loop through the page frames to find the corresponding dirty page
    for (; currentIndex < pool->numPages; currentIndex++)
    {
        if (pageFrames[currentIndex].buffer_handle->pageNum == handle->pageNum && pageFrames[currentIndex].is_dirtyFlag)
        {
            pageExists = true;
            break;
        }
    }

    // Return an error if the dirty page was not found
    if (!pageExists)
    {
        RC_message = "The given page number is not dirty.";
        return RC_ERROR;
    }

    // Clean the page and write its content to disk
    openPageFile(pool->pageFile, &handleFile);
    pageFrames[currentIndex].read_content = handle->data;
    ensureCapacity(handle->pageNum, &handleFile);
    writeBlock(handle->pageNum, &handleFile, pageFrames[currentIndex].read_content);
    pageFrames[currentIndex].is_dirtyFlag = 0;

    RC_message = "Page successfully flushed to disk.";
    return RC_OK;
}

//making pages dirty
RC markDirty(BM_BufferPool *const bufferPool, BM_PageHandle *const pageHandle)
{
    // Retrieve the page frame nodes from the buffer pool management data
    PF_node *frameNodes = (PF_node *)bufferPool->mgmtData;
    bool dirtyFlagSet = false;
    int index = 0;
    // Loop through the page frames to locate the matching page
    while (index < bufferPool->numPages && !dirtyFlagSet)
    {
        if (frameNodes[index].buffer_handle->pageNum == pageHandle->pageNum)
        {
            frameNodes[index].is_dirtyFlag = 1; // Mark the page as dirty
            write_Count++; // Increment write count
            dirtyFlagSet = true; // Set the flag indicating the page is dirty
        }
        index++;
    }

    // Set the appropriate message based on the result
    if (dirtyFlagSet) {
    RC_message = "Page noted as dirty.";
    return RC_OK;
   } else {
    RC_message = "Page number does not exist in buffer pool.";
    return RC_ERROR;
   }

}


RC unpinPage(BM_BufferPool *const bufferPool, BM_PageHandle *const pageHandle)
{
    // Retrieve the page frame nodes from the buffer pool management data
    PF_node *frameNodes = (PF_node *)bufferPool->mgmtData;
    int index = 0;
    bool isUnpinned = false;

    // Loop through the page frames to locate the matching page
    while (index < bufferPool->numPages && !isUnpinned)
    {
        if (frameNodes[index].buffer_handle->pageNum == pageHandle->pageNum && frameNodes[index].fix_counter > 0)
        {
            frameNodes[index].fix_counter--; // Decrement the fix counter
            isUnpinned = true; // Set the flag indicating the page was unpinned
        }
        index++;
    }

    // Determine the message to be set based on the outcome
   if (isUnpinned) {
    RC_message = "Page unpinned successfully.";
    return RC_OK; // Return success status
   } else {
    RC_message = "Unpinning failed: page number does not exist in the buffer pool.";
    return RC_ERROR; // Return error status
   }

}

// Exit Strategies //

bool pinFIFO(BM_BufferPool *const bufferPool, BM_PageHandle *const pageHandle, const PageNumber pageNum)
{
    int currentPageNum;
    bool isPinned = false;
    Queue *pageQueue = (Queue *)pf_metadata.queue;
    SM_FileHandle fileHandle;
    PF_node *frames = (PF_node *)bufferPool->mgmtData;
    
    int frameIndex = 0;
    while (frameIndex < bufferPool->numPages && !isPinned)
{
    currentPageNum = pageQueue[frameIndex].pageNumber;

    for (int frameSearchIndex = 0; frameSearchIndex < bufferPool->numPages; frameSearchIndex++)
    {
        PF_node *currentFrame = &frames[frameSearchIndex];  // Pointer to the current frame
        BM_PageHandle *bufferHandle = currentFrame->buffer_handle;

        // Check if the page number matches and the frame is not pinned
        if (bufferHandle->pageNum == currentPageNum && currentFrame->fix_counter == 0)
        {
            // Open the page file
            openPageFile(bufferPool->pageFile, &fileHandle);

            // Ensure capacity or handle a dirty page
            if (!currentFrame->is_dirtyFlag)
            {
                ensureCapacity(pageNum, &fileHandle);
            }
            else
            {
                // Write the dirty page back to disk
                SM_PageHandle currentContent = bufferHandle->data;
                currentFrame->read_content = currentContent;
                writeBlock(bufferHandle->pageNum, &fileHandle, currentContent);
            }

            // Read the block from disk into the frame
            readBlock(pageNum, &fileHandle, currentFrame->read_content);

            // Assign values to the buffer page handle and frame
            pageHandle->pageNum = bufferHandle->pageNum = pageNum;
            pageHandle->data = bufferHandle->data = currentFrame->read_content;
            currentFrame->is_dirtyFlag = 0;  // Mark as clean
            currentFrame->fix_counter = 1;   // Increment fix count

            // Update the queue to move this page to the most recently used position
            for (int z = 1; z < bufferPool->numPages; z++)
            {
                pageQueue[z - 1] = pageQueue[z];  // Shift the queue
            }
            pageQueue[bufferPool->numPages - 1].pageNumber = pageNum;
            pageQueue[bufferPool->numPages - 1].frameNumber = currentFrame->Frame_Idx;

            // Increment the read count
            read_Count++;

            // Set the flag indicating the page has been pinned
            isPinned = true;
            break;
        }
    }

    frameIndex++;
}

    return isPinned;
}


bool pinLRU(BM_BufferPool *const bufferPool, BM_PageHandle *const pageHandle, const PageNumber pageNum)
{
    int currentPageNum;
    bool isPinned = false;  // Track if the page was pinned successfully
    PF_node *frames = (PF_node *)bufferPool->mgmtData;  // Access frame nodes
    SM_FileHandle fileHandle;  // Handle for file operations
    Queue *pageQueue = (Queue *)pf_metadata.queue;  // Access page queue

    // Loop through the queue and attempt to pin the page
    for (int queueIndex = 0; queueIndex < bufferPool->numPages && !isPinned; queueIndex++)
    {
        currentPageNum = pageQueue[queueIndex].pageNumber;  // Get current page number from queue

        // Loop through frames to find the frame with the same page number
        for (int frameIndex = 0; frameIndex < bufferPool->numPages && !isPinned; frameIndex++)
        {
            PF_node *currentFrame = &frames[frameIndex];  // Reference current frame
            bool isFrameAvailable = (currentFrame->buffer_handle->pageNum == currentPageNum &&
                                     currentFrame->fix_counter == 0);

            if (isFrameAvailable)
            {
                // Handle non-dirty pages
                if (!currentFrame->is_dirtyFlag)
                {
                    // Open page file and ensure capacity for the page
                    openPageFile(bufferPool->pageFile, &fileHandle);
                    ensureCapacity(pageNum, &fileHandle);
                }
                else
                {
                    // Force dirty pages to be written back to disk
                    forcePage(bufferPool, currentFrame->buffer_handle);
                }

                // Read the page content into the frame's buffer
                readBlock(pageNum, &fileHandle, currentFrame->read_content);

                // Update the page handle and frame information
                frames[frameIndex].buffer_handle->pageNum = pageNum;        // Set the page number in the buffer handle
                frames[frameIndex].buffer_handle->data = frames[frameIndex].read_content; // Update the buffer handle data
                pageHandle->pageNum = pageNum;                              // Update the page handle with the new page number
                pageHandle->data = frames[frameIndex].read_content;        // Set the data in the page handle to the read content
                frames[frameIndex].is_dirtyFlag = 0;// Mark the frame as clean
                frames[frameIndex].fix_counter = 0;  // Reset fix counter

                // Shift the queue to make space for the newly pinned page
                for (int z = queueIndex + 1; z < bufferPool->numPages; z++)
                {
                    pageQueue[z - 1] = pageQueue[z]; // Shift pages in the queue
                }

                // Place the new page number and frame index at the end of the queue
                int lastIndex = bufferPool->numPages - 1;
                pageQueue[lastIndex].pageNumber = pageNum;
                pageQueue[lastIndex].frameNumber = frames[frameIndex].Frame_Idx;

                // Increment the read count as a page was successfully read
                read_Count++;

                // Set the flag indicating the page has been pinned
                isPinned = true;
            }
        }
    }

    return isPinned; // Return whether the page was successfully pinned
}


bool pinCLOCK(BM_BufferPool *const bufferPool, BM_PageHandle *const pageHandle, const PageNumber pageNum)
{
    int currentPageNum;
    bool isPinned = false;
    Queue *pageQueue = (Queue *)pf_metadata.queue;
    SM_FileHandle fileHandle;
    PF_node *frames = (PF_node *)bufferPool->mgmtData;

    int queueIndex = 0;

    while (queueIndex < bufferPool->numPages && !isPinned) {
        currentPageNum = pageQueue[queueIndex].pageNumber;
        int frameIndex = clock_Idx; // Start from the last used index

        while (frameIndex < bufferPool->numPages) {
            PF_node *currentFrame = &frames[frameIndex];

            // Check if the current frame matches the required page and is not pinned
            if (currentFrame->buffer_handle->pageNum == currentPageNum && currentFrame->fix_counter == 0) {
                if (currentFrame->is_usedFlag == 1) { // Reset flag if used
                    currentFrame->is_usedFlag = 0; // Mark as unused for the next iteration
                    frameIndex++;
                    continue; // Check the next frame
                }

                // Mark the frame as used
                currentFrame->is_usedFlag = 1;

                openPageFile(bufferPool->pageFile, &fileHandle);
                if (currentFrame->is_dirtyFlag == 1) {
                    // Write the dirty page back to the file
                    forcePage(bufferPool, currentFrame->buffer_handle);
                } else {
                    // Ensure capacity for the new page
                    ensureCapacity(pageNum, &fileHandle);
                }

                // Read the content into the frame
                readBlock(pageNum, &fileHandle, currentFrame->read_content);

                // Update the page handle and current frame details
                    // Assign the page number to both the page handle and the frame's buffer
                pageHandle->pageNum = pageNum;
                currentFrame->buffer_handle->pageNum = pageNum;

                pageHandle->data = currentFrame->read_content;
                currentFrame->buffer_handle->data = currentFrame->read_content;

                currentFrame->is_dirtyFlag = false;
                currentFrame->fix_counter = 1;  // Set the fix counter to indicate the page is pinned

                // Update the page queue
                int tempPageNumber = pageQueue[frameIndex].pageNumber;
                int tempFrameNumber = pageQueue[frameIndex].frameNumber;

                // Replace the page number and frame number in the queue
                pageQueue[queueIndex].pageNumber = tempPageNumber;
                pageQueue[queueIndex].frameNumber = tempFrameNumber;

                // Move the new page to the end of the queue
                int lastIndex = bufferPool->numPages - 1;
                pageQueue[lastIndex].pageNumber = pageNum;
                pageQueue[lastIndex].frameNumber = currentFrame->Frame_Idx;

                // Increment read count and update clock index
                read_Count++;
                clock_Idx = frameIndex+1;

                // Indicate that the page has been pinned successfully
                isPinned = true;
                break; // Break out of the inner while loop
            }
            frameIndex=frameIndex+1; // Move to the next frame
        }
        queueIndex=queueIndex+1; // Move to the next queue index
    }

    return isPinned; // Return the result of the pin operation  
}


RC pinPage(BM_BufferPool *const bufferPool, BM_PageHandle *const pageHandle, const PageNumber pageNum)
{
    // Validate input: check if the buffer pool exists and if the page number is valid
    // Check for invalid buffer pool management data or negative page number
    if (bufferPool->mgmtData == NULL) 
    {
        return RC_ERROR;  // Return error if management data is missing
    }

    if (pageNum < 0) 
    {
        return RC_ERROR;  // Return error for invalid page number
    }


    bool pageFound = false;  // Flag to check if the requested page is found
    Queue *pageQueue = (Queue *)pf_metadata.queue;  // Retrieve the page queue
    SM_FileHandle fileHandle;  // File handle for page operations
    PF_node *frames = (PF_node *)bufferPool->mgmtData;  // Retrieve buffer frames

    // Iterate through the queue to check if the requested page is already in the buffer
    for (int i = 0; i < bufferPool->numPages; i++)
    {
        // Check if the current queue entry matches the requested page number
    if (pageQueue[i].pageNumber == pageNum) 
    {
        // Get the corresponding frame index
        int frameIdx = pageQueue[i].frameNumber;  
        // Access the buffer handle
        BM_PageHandle *bufHandle = frames[frameIdx].buffer_handle;  

        // Set the page handle properties from the buffer frame
        pageHandle->pageNum = bufHandle->pageNum;
        pageHandle->data = bufHandle->data;

        // Increment the fix count for the frame
        frames[frameIdx].fix_counter += 1;

        // Maintain the order in the queue by shifting entries
        int j = i + 1;
        while (j < bufferPool->numPages) 
        {
            pageQueue[j - 1] = pageQueue[j];  // Shift the queue entries
            j++;
        }

        // Move the accessed page to the end of the queue
        int last = bufferPool->numPages - 1;
        pageQueue[last].pageNumber = pageNum;  
        pageQueue[last].frameNumber = frames[frameIdx].Frame_Idx;  

        // Mark the page as found and exit the loop
        pageFound = true;
        break;  // Break out of the loop
    }

    }
        if (pageFound)
    {
        return RC_OK; // If the page is already found, return success immediately
    }
    else
    {
    // Verify if the buffer pool has reached its capacity
    if (pf_metadata.total_Frames_filled == pf_metadata.Frames_count) 
{
    bool isPinned = false; // Track pinning status

    // Determine pinning strategy using if-else
    if (bufferPool->strategy == RS_FIFO) 
    {
        isPinned = pinFIFO(bufferPool, pageHandle, pageNum); // FIFO strategy
    } 
    else if (bufferPool->strategy == RS_LRU) 
    {
        isPinned = pinLRU(bufferPool, pageHandle, pageNum); // LRU strategy
    } 
    else if (bufferPool->strategy == RS_LRU_K) 
    {
        isPinned = pinLRU(bufferPool, pageHandle, pageNum); // LRU_K strategy
    } 
    else if (bufferPool->strategy == RS_CLOCK) 
    {
        isPinned = pinCLOCK(bufferPool, pageHandle, pageNum); // CLOCK strategy
    } 
    else 
    {
        // Optionally handle unrecognized strategies
    }

    // Return the result of the pinning operation
    return (isPinned) ? RC_OK : RC_ERROR; // Return success or error
}

    else
    {
        // Iterate through the buffer pool frames to find an empty slot
        for (int i = 0; i < bufferPool->numPages; i++)
        {
            // Check if the current frame is empty
            if (frames[i].buffer_handle->pageNum == -1) {
    // Open the page file to prepare for reading
    openPageFile(bufferPool->pageFile, &fileHandle);

    // Ensure there is enough space in the page file for the requested page
    ensureCapacity(pageNum, &fileHandle);

    // Pointer to where the current content will be read into
    SM_PageHandle currentPageContent = frames[i].read_content;

    // Read the specified block from the page file into the buffer
    readBlock(pageNum, &fileHandle, currentPageContent);

    // Update the buffer page handler's properties
    frames[i].is_dirtyFlag = 0;           // Mark the frame as not dirty
    frames[i].fix_counter = 1;             // Set the fix count to indicate the page is pinned
    pageHandle->data = currentPageContent; // Set the page handle's data to the current content
    pageHandle->pageNum = pageNum;         // Set the page handle's page number
    frames[i].buffer_handle->data = currentPageContent; // Update the frame's buffer handle data
    frames[i].buffer_handle->pageNum = pageNum; // Update the page number in the buffer handle

    // Update the page queue to reflect the newly pinned frame
    pageQueue[i].frameNumber = frames[i].Frame_Idx; // Store the frame index
    pageQueue[i].pageNumber = pageNum; // Store the page number in the queue

    // Increment the total number of frames filled in the metadata
    pf_metadata.total_Frames_filled++;

    // Increment the read operation count
    read_Count++;

    return RC_OK; // Return success status indicating the page was successfully pinned
}
        }
    }
}

return RC_ERROR; // Return an error if pinning was unsuccessful
}
int getNumWriteIO (BM_BufferPool *const bm)
{
   return write_Count;
}

//returns total number of reads
int getNumReadIO (BM_BufferPool *const bm)
{
   return read_Count;
}
//returns array with all the page numbers in the buffer pool.
PageNumber *getFrameContents(BM_BufferPool *const bm)
{
    PageNumber *page_Idxs = malloc(sizeof(PageNumber) * bm->numPages);
    PF_node *page_frame_node = (PF_node *)bm->mgmtData;

    // Use a for loop for better readability
    for (int i = 0; i < bm->numPages; i++)
    {
        if (page_frame_node[i].buffer_handle->pageNum == -1)
        {
            page_Idxs[i] = NO_PAGE;  // Assign NO_PAGE if page number is -1
        }
        else
        {
            page_Idxs[i] = page_frame_node[i].buffer_handle->pageNum;  // Assign the actual page number
        }
    }

    return page_Idxs;
}



//returns array with dirty flags for each frame in the buffer pool.
bool *getDirtyFlags(BM_BufferPool *const bm)
{
    bool *is_dirty = malloc(sizeof(bool) * bm->numPages);
    PF_node *page_frame_node = (PF_node *)bm->mgmtData;

    // Use a for loop for better readability
    for (int i = 0; i < bm->numPages; i++)
    {
        is_dirty[i] = (page_frame_node[i].is_dirtyFlag != 0);  // Set dirty flag
    }

    return is_dirty; // Return the array
}


//returns array with all the fixed counts for each frame in the buffer pool.
int *getFixCounts(BM_BufferPool *const bm)
{
    int *fix_Idx = malloc(sizeof(int) * bm->numPages);
    PF_node *page_frame_node = (PF_node *)bm->mgmtData;

    // Use a for loop for better readability
    for (int i = 0; i < bm->numPages; i++)
    {
        // Assign the fix count or NO_PAGE if fix_counter is -1
        fix_Idx[i] = (page_frame_node[i].fix_counter == -1) ? NO_PAGE : page_frame_node[i].fix_counter;
    }

    return fix_Idx; // Return the array
}




