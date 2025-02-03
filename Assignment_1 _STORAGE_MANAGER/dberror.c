#include "dberror.h"
#include <stdlib.h>
#include <string.h>

// Error message buffer
static char errorBuffer[512] = {0};

// Set the error message
void setErrorMessage(const char *message) {
    strncpy(errorBuffer, message, sizeof(errorBuffer) - 1);
}

// Retrieve the error message
const char *getErrorMessage(void) {
    return errorBuffer;
}

// Print error details
void displayError(RC errorCode) {
    if (strlen(errorBuffer) > 0) {
        printf("Error [%d]: %s\n", errorCode, errorBuffer);
    } else {
        printf("Error [%d]: No additional details.\n", errorCode);
    }
}
