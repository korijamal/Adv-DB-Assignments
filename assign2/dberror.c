#include "dberror.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *RC_message;

void printError(RC error) {
    if (RC_message)
        printf("Error Code (%d): \"%s\"\n", error, RC_message);
    else
        printf("Error Code (%d)\n", error);
}
