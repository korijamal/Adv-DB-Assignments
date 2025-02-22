# Buffer Manager Implementation

## 📌 Overview
This project implements a **Buffer Manager** that supports **FIFO, LRU, CLOCK, LFU, and LRU-K** page replacement strategies. It interacts with a **Storage Manager** for reading/writing pages and tracks statistics such as page reference counts.

---

## 📂 File Structure

| **File**               | **Description** |
|------------------------|----------------|
| `Makefile`            | Compiles and links all necessary files to produce executables. |
| `buffer_mgr.h`        | Header file for buffer manager functions and data structures. |
| `buffer_mgr.c`        | Implements buffer pool and page handling functions. |
| `buffer_mgr_stat.c`   | Implements buffer pool statistics functions. |
| `dberror.h`           | Defines error codes for handling failures. |
| `dberror.c`           | Implements error handling and debugging utilities. |
| `dt.h`               | Defines basic data types including `PageNumber` and `bool`. |
| `storage_mgr.h`       | Defines the storage manager interface. |
| `storage_mgr.c`       | Implements file operations (create, read, write, delete). |
| `test_assign2_1.c`    | Tests **FIFO** buffer replacement strategy. |
| `test_assign2_2.c`    | Tests **LRU, CLOCK, and error handling**. |
| `test_helper.h`       | Defines helper macros for test cases. |

---

## **How to Compile and Run**
### **1️⃣ Clean Old Builds**
```sh
make clean

###Run FIFO test case:

./test_case1

###Run LRU & CLOCK test case:

./test_case2


🛠️ Features Implemented
1️⃣ Buffer Pool Initialization

    initBufferPool → Creates a new buffer pool with a specific page file, page size, and replacement strategy.
    shutdownBufferPool → Releases all resources associated with the buffer pool.
    forceFlushPool → Writes all dirty pages back to disk.

2️⃣ Page Management

    pinPage → Loads a page into the buffer pool.
    unpinPage → Releases a page from the buffer pool.
    forcePage → Writes a dirty page to disk immediately.
    markDirty → Marks a page as modified.

3️⃣ Replacement Strategies Implemented

    FIFO (First-In-First-Out)
    LRU (Least Recently Used)
    CLOCK (Clock Page Replacement Algorithm)
    LRU-K (Least Recently Used - K distance tracking)

4️⃣ Buffer Pool Statistics

    getFrameContents → Retrieves the list of pages in the buffer pool.
    getDirtyFlags → Identifies which pages are dirty.
    getFixCounts → Tracks the fix count for each page.
    getNumReadIO → Counts the number of pages read from disk.
    getNumWriteIO → Counts the number of pages written to disk.

5️⃣ Error Handling

    Handles invalid buffer pool operations (e.g., pinning an out-of-range page).
    Detects memory allocation failures and provides proper error messages.
    Ensures safe shutdown of buffer pools to prevent memory leaks.

📖 Code Breakdown
🔹 Part 1: Buffer Pool Initialization
initBufferPool(BM_BufferPool *const bm, const char *const pageFileName, const int numPages, ReplacementStrategy strategy, void *stratData)

    Allocates memory for the buffer pool.
    Sets the replacement strategy and initializes necessary data structures.

shutdownBufferPool(BM_BufferPool *const bm)

    Frees all memory allocated for the buffer pool.
    Writes dirty pages to disk before shutting down.

forceFlushPool(BM_BufferPool *const bm)

    Iterates through the buffer pool and writes all dirty pages to disk.

🔹 Part 2: Page Access Interface
markDirty(BM_BufferPool *const bm, BM_PageHandle *const page)

    Marks a page as dirty so it gets written back to disk.

unpinPage(BM_BufferPool *const bm, BM_PageHandle *const page)

    Decreases the fix count of a page, allowing it to be replaced if necessary.

forcePage(BM_BufferPool *const bm, BM_PageHandle *const page)

    Immediately writes a dirty page to disk.

pinPage(BM_BufferPool *const bm, BM_PageHandle *const page, const PageNumber pageNum)

    Loads a requested page into the buffer pool.

🔹 Part 3: Replacement Strategies
FIFO(BM_BufferPool *const bm, BM_PageHandle *const page)

    Implements First-In-First-Out (FIFO) page replacement.

LRU(BM_BufferPool *const bm, BM_PageHandle *const page)

    Implements Least Recently Used (LRU) replacement.

LRU_k(BM_BufferPool *const bm, BM_PageHandle *const page)

    Implements LRU-K where the last k accesses are considered.

CLOCK(BM_BufferPool *const bm, BM_PageHandle *const page)

    Implements CLOCK page replacement algorithm.

🔹 Part 4: Statistics Interface
getFrameContents(BM_BufferPool *const bm)

    Retrieves an array containing the current pages in memory.

getDirtyFlags(BM_BufferPool *const bm)

    Returns an array indicating which pages are dirty.

getFixCounts(BM_BufferPool *const bm)

    Returns an array showing how many times each page is pinned.

getNumReadIO(BM_BufferPool *const bm)

    Returns the number of pages read from disk.

getNumWriteIO(BM_BufferPool *const bm)

    Returns the number of pages written to disk.

🛠️ Memory Leak Check
For MacOS Users:

Run the following command to check for memory leaks:

leaks -atExit -- ./test_case1 | grep LEAK:

leaks -atExit -- ./test_case2 | grep LEAK:

Expected Output:

Process XXXX is not debuggable. No memory leaks detected.

Loom Video Link:

https://www.loom.com/share/97545ee8b0244b788a4287fcbaeca9ea?sid=69f96f65-9b68-48d1-891f-a92c20ee37f6