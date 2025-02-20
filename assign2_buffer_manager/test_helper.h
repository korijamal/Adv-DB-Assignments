#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include "dberror.h"
#include <stdlib.h>

#define CHECK(code) \
    do { \
        int rc_internal = (code); \
        if (rc_internal != RC_OK) { \
            printError(rc_internal); \
            exit(1); \
        } \
    } while (0)

#endif
