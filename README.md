# CS 525: Advance Database Organization

## PROGRAMMING ASSIGNMENT 1: STORAGE MANAGER 

### Team Members:
• Kori Kamoljonov - A20594489 - 33.3% Contributed
• Chaitanya Datta Maddukuri - A20568393 - 33.3% Contributed
• Vamshi Krishna Cheeti - A20582646 - 33.4% Contributed

---

## README- Store Manager

This C program implements a **storage manager** for database file operations. It supports creating, opening, closing, deleting files, and reading fixed-size pages (4096 bytes). Error handling ensures robustness for invalid operations. 

---

## How to Run:

### Step 1: Compile the Code  
1. **Download Files**: Ensure these files are in the same directory:  
   - `storage_mgr.h`, `dberror.h`, `test_helper.h`  
   - `dberror.c`, `storage_mgr.c`, `test_assign1_1.c`  

2. **Compile with GCC**:  
   ```bash  
   gcc dberror.c storage_mgr.c test_assign1_1.c -o storage_manager_test  
### Step 2: Remove any previously compiled files.
  
    make clean  

### Step 3:  Compile the program by running the Makefile.  

    make 

### Step 4:  Execute the test cases.

    ./test_17

### Step 5:  Once again, use "make clean" to remove generated executable files, and repeat steps 3 and 4 to rerun the assignment.

---

## Features

1. File Creation: Initialize a new file with an empty page.
2. File Operations: Open, close, and delete files.
3. Page Reading: Read specific pages from files.
4. Error Handling: Gracefully handle missing files, invalid pages, and I/O errors.

---

## File-Related Functions

### Part 1: File Management

This section details the functions used to create, open, close, and delete/destroy page files.

- Initializes the storage manager.

        void initStorageManager()

- Creates a new file with one empty page.

        RESULT_CODE createNewFile(const char *filePath)

- Opens a file and initializes metadata (total pages, current position).

        RESULT_CODE openExistingFile(const char *filePath, FileManager *fileManager)

- Closes an open file.

        RESULT_CODE closeFile(FileManager *fileManager)

- Deletes a file from the filesystem.

        RESULT_CODE removeFile(const char *filePath)


---

### Part 2: Page Reading and Writing

This section explains how to read different blocks from the page files.

- Reads a page into a buffer. Validates page bounds.
  
        RESULT_CODE readPage(int pageNumber, FileManager *fileManager, char *buffer)
- Reads the first page of the file.
  
        RESULT_CODE readFirstPage(FileManager *fileManager, char *buffer)
- Reads the page immediately before the current page.

        RESULT_CODE readPreviousPage(FileManager *fileManager, char *buffer)
- Reads the page at the current position.

        RESULT_CODE readCurrentPage(FileManager *fileManager, char *buffer)
- Reads the page immediately after the current page.

        RESULT_CODE readNextPage(FileManager *fileManager, char *buffer)
- Reads the last page in the file.

        RESULT_CODE readLastPage(FileManager *fileManager, char *buffer)

---

This section describes how to write blocks to page files. 

- Writes data to a specified page in the file.

        RESULT_CODE writePage(int pageNumber, FileManager *fileManager, const char *data)

- Writes data to the current page position.

        RESULT_CODE writeCurrentPage(FileManager *fileManager, const char *data)

- Appends an empty page (filled with zero bytes) to the end of the file.

        RESULT_CODE appendEmptyPage(FileManager *fileManager)

- Ensures that the file contains at least a specified number of pages by appending empty pages if necessary.

        RESULT_CODE ensureCapacity(int numberOfPages, FileManager *fileManager)

---

## Code Structure

- Core file/page management logic.

        storage_mgr.h/storage_mgr.c

- Error codes, messages, and utilities.

        dberror.h/dberror.c

- Test cases for file operations.

        test_assign1_1.c

- Test assertion macros.

        test_helper.h


> [!NOTE]
> 1. The test creates/deletes test_file.bin. Ensure no conflicts with existing files.


> [!IMPORTANT] 
> This README provides everything needed to compile, run, and understand the storage manager.
