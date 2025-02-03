#ifndef DBERROR_H
#define DBERROR_H

#include <stdio.h>

// Page size
#define PAGE_SIZE 4096

// Return Code Type
typedef int RC;

// Standard Return Codes
#define RC_OK 0
#define RC_FILE_NOT_FOUND 1
#define RC_FILE_HANDLE_NOT_INIT 2
#define RC_WRITE_FAILED 3
#define RC_READ_NON_EXISTING_PAGE 4

// Additional Error Codes
#define RC_INVALID_DATATYPE 200
#define RC_BOOLEAN_EXPECTED 201
#define RC_NO_MORE_RECORDS 202
#define RC_UNKNOWN_DATATYPE 205

#define RC_INDEX_KEY_NOT_FOUND 300
#define RC_INDEX_DUPLICATE_KEY 301
#define RC_INDEX_OVERFLOW 302
#define RC_INDEX_NO_ENTRIES 303

// Function Prototypes
extern void setErrorMessage(const char *message);
extern const char *getErrorMessage(void);
extern void displayError(RC errorCode);

// Macros for error handling
#define THROW_ERROR(code, message) \
    do {                           \
        setErrorMessage(message);  \
        return code;               \
    } while (0)

#endif
