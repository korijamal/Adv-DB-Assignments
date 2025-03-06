#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include <stdio.h>
#include <stdlib.h>

// Macro to check test assertions
#define ASSERT_TRUE(condition, message) \
    do { \
        if (!(condition)) { \
            printf("[FAILED] %s | Line: %d | File: %s\n", message, __LINE__, __FILE__); \
            exit(EXIT_FAILURE); \
        } else { \
            printf("[PASSED] %s\n", message); \
        } \
    } while (0)

// Macro to validate function return codes
#define TEST_CHECK(code) \
    do { \
        int rc = (code); \
        if (rc != RC_OK) { \
            printf("[ERROR] Test failed with error code %d at %s: Line %d\n", rc, __FILE__, __LINE__); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

// Print test completion message
#define TEST_DONE() \
    do { \
        printf("Test Completed Successfully!\n"); \
    } while (0)

#endif
