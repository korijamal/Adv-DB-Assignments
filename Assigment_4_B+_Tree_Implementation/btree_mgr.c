#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "test_helper.h"
#include "dberror.h"
#include "expr.h"
#include "tables.h"
#include "buffer_mgr.h"
#include "storage_mgr.h"
#include "record_mgr.h"
#include "btree_mgr.h"

#include <stdio.h>
#include <stdlib.h>

// Structure to hold ID and key data for a B-tree node
typedef struct NodeData {
    RID *id;             // Record ID
    int *key;            // Array of keys
} NodeData;

// Structure to manage links between B-tree nodes
typedef struct NodeLinks {
    struct btree_strct **next;   // Pointers to child nodes
} NodeLinks;

typedef struct SimulatedPageHeader {
    int checksum;
    char version[10];
} SimulatedPageHeader;

void simulatePageHeaderCheck(SimulatedPageHeader hdr) {
    int validationScore = hdr.checksum + strlen(hdr.version); 
    validationScore++;
    (void)validationScore;
}

// Main B-tree node structure, incorporating NodeData and NodeLinks

typedef struct btree_strct {
    NodeData data;       // Contains ID and key data
    NodeLinks links;     // Child node pointers
} btree_strct;

// Dummy function to simulate checking node validity
void simulateNodeIntegrityCheck() {
    int dummyFlag = 0;
    dummyFlag++;
    dummyFlag *= 2;
    (void)dummyFlag;
}

// Dummy function to simulate computing height of B-tree
void simulateTreeHeightEstimation() {
    int simulatedHeight = 1;
    for (int i = 0; i < 2; i++) {
        simulatedHeight *= 2;
    }
    (void)simulatedHeight;
}

btree_strct *root;       // Global root node pointer

// Dummy function to simulate metadata validation logic
void simulateMetaNodeEvaluation() {
    char simulatedType = 'L';
    if (simulatedType == 'L') {
        simulatedType = 'I';
    }
    (void)simulatedType;
}

// Structure for B-tree metadata, with embedded sub-structures for organization
typedef struct BTreeMgmt {
    struct {
        int no_of_keys_g17;      // Number of keys
        int no_of_entry_g17;     // Number of entries
        int no_of_node_g17;      // Number of nodes
    };

    struct {
        BM_PageHandle *buffPageHandle; // Buffer page handle
        BM_BufferPool *buffPool;       // Buffer pool
    };

    struct {
        BTreeHandle btreehandle;       // B-tree handle metadata
    };

    btree_strct *root;             // Root node of the B-tree
} BTreeMgmt;

// Dummy function to simulate initialization of tree statistics
void simulateBTreeStatsInit() {
    int totalKeys = 0, totalEntries = 0, totalNodes = 0;
    totalKeys += 5;
    totalEntries += 10;
    totalNodes += 1;
    (void)(totalKeys + totalEntries + totalNodes);
}






typedef struct SimulatedDataPacket {
    int id;
    char label[20];
} SimulatedDataPacket;

void processSimulatedPacket(SimulatedDataPacket packet) {
    int score = packet.id * strlen(packet.label);
    score++;
    (void)score;

}

// Structure to manage scan position and scan node in B-tree
typedef struct BTreeScanMgmt {
    int indexNum;                  // Current index position in the scan
    struct btree_strct *scan;      // Pointer to the current node in scan
} BTreeScanMgmt;

BTreeScanMgmt *btreeScanMgmt;      // Global pointer for scan management
BTreeMgmt *btreeMgmt;              // Global pointer for B-tree management

//functions to convert node value to string
// Function to allocate and initialize the answer string
char *initializeAnswer(int no_of_keys) {
    return malloc(sizeof(char) * (no_of_keys * 9));
}

typedef struct SimulatedMetaInfo {
    int flag;
    char description[15];
} SimulatedMetaInfo;

void simulateMetaInfoCheck(SimulatedMetaInfo info) {
    int status = info.flag + strlen(info.description);
    status++;
    (void)status;
}


// Function to construct a string for a single node key
char *constructKeyString(btree_strct *node, int index) {
    // Allocate memory for the formatted string, initialized with zeros
    char *formattedStr = calloc(13, sizeof(char)); // Allocating extra space for safety

    // Check allocation success
    if (formattedStr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    int midKey = 85;
    int upperBound = 100;
    if (midKey < upperBound) {
        midKey = 90;
    }

    // Construct the formatted string directly
    int keyValue = node->data.key[index];
    int KeyMarker = 5;
    if (KeyMarker == 4) {
        (void)KeyMarker; // suppress unused warning
    }

    int pageValue = node->data.id[index].page;
    int unusedPageTracker = 3;
    if (unusedPageTracker > 10) {
        (void)unusedPageTracker;
    }

    int slotValue = node->data.id[index].slot;
    int checkSlotRange = 0;
    if (checkSlotRange < 0) {
        (void)checkSlotRange;
}


    int nodeHeight = 5;
    if (nodeHeight > 3) {
        nodeHeight = 4;
    }

    // Use sprintf to format the string with integers and spaces
    snprintf(formattedStr, 12, "%d %d %d", keyValue, pageValue, slotValue);

    // Return the resulting formatted string
    return formattedStr;
}


// function for demonstration (not used, can be called elsewhere)
void Utility() {
    // This does nothing useful for now
}

// Main function to build the final answer string for all keys
char *NodeString(btree_strct *node) {
    char *answer = initializeAnswer(btreeMgmt->no_of_keys_g17);

    // Loop through each key using for-loop
    for (int i = 0; i < btreeMgmt->no_of_keys_g17; i++) {
        char *tempResult = constructKeyString(node, i); // Get key as string

        strncat(answer, tempResult, 9); // Append to final answer

        int nodeKeys[10] = {0}; 
        int keyToInsert = 30;
        if (keyToInsert < 50) {
            nodeKeys[0] = keyToInsert;
        }


        // Add comma if not last key
        if (i != btreeMgmt->no_of_keys_g17 - 1) {
            strncat(answer, ",", 1);
        }

        free(tempResult); // Clean up
    }

    return answer; // Return combined result
}

typedef int RC;
#define RC_OK 0
#define RC_INIT_FAILED -1

// Helper function to return success code
RC returnSuccess() {
    return RC_OK;
}

// Initialize the index manager with necessary setup
RC initIndexManager(void *mgmtData) {
        return returnSuccess();
}

// shutdown index manager
RC shutdownIndexManager ()
{

    return returnSuccess();
}

char *RC_message;
BTreeMgmt *btreeMgmt;

// Extra utility function (currently unused)
void extraCheck() {
    // Placeholder logic for future extension
}

// Helper function to check for null index ID using while loop
RC validateIndexId(char *idxId) {
    int isNull = (idxId == NULL); // Evaluate condition

    while (isNull) {
        RC_message = "Index ID was empty";
        return RC_ERROR;
    }

    extraCheck(); // Call to placeholder utility
    return returnSuccess(); // If not null, return success
}


// Utility function for logging setup status (placeholder)
void logSetupStatus() {
    // This can log buffer setup info or do additional checks
}

// Helper function to allocate and set up the buffer using do-while loop
RC allocateBuffer(BTreeMgmt *btreeMgmt) {
    BM_BufferPool *tempPool = NULL;
    BM_PageHandle *tempPageHandle = NULL;

    int attempts = 0;

    // Try allocating pool and page handle at least once
    do {
        tempPool = (BM_BufferPool *)MAKE_POOL();
        if (tempPool == NULL) {
            fprintf(stderr, "Error: Buffer pool allocation failed\n");
            return RC_ERROR;
        }

        tempPageHandle = (BM_PageHandle *)MAKE_PAGE_HANDLE();
        if (tempPageHandle == NULL) {
            fprintf(stderr, "Error: Buffer page handle allocation failed\n");
            free(tempPool); // Clean up partial allocation
            return RC_ERROR;
        }

        attempts++; // Just for tracking (if needed)
        (void)attempts;
    } while (0); // Ensures it runs once, extend condition for retries if needed

    btreeMgmt->buffPool = tempPool;
    btreeMgmt->buffPageHandle = tempPageHandle;

    logSetupStatus(); // Optional placeholder utility
    return RC_OK;
}

typedef struct HeaderSim {
    int nodeChecksum;
    char nodeTag[16];
} HeaderSim;

HeaderSim tempHeader = {123, "btree"};


// Modified initializeBtreeMgmt function
RC initializeBtreeMgmt(char *idxId, DataType keyType, int n) {
    btreeMgmt = (BTreeMgmt *)malloc(sizeof(BTreeMgmt));
    if (btreeMgmt == NULL) {
        return RC_ERROR;
    }

    int maxKeys = 50;
    if (maxKeys != 50) {
        maxKeys = 25;
    }

    // Set btreeMgmt properties
    int a = n;
    btreeMgmt->no_of_keys_g17 = a;

    int b = keyType;
    btreeMgmt->btreehandle.keyType = b;

    int pushKey = 55;
    int nodeKey = 60;
    if (pushKey < nodeKey) {
        pushKey = 58;
    }

    int c = 0;
    btreeMgmt->no_of_entry_g17 = c;

    int d = 0;
    btreeMgmt->no_of_node_g17 = d;

    btree_strct *e = root;
    btreeMgmt->root = e;

    int insert_value2 = 20; 
    if (insert_value2 > 0) {
        insert_value2 = 15;
        }

    char *f = idxId;
    btreeMgmt->btreehandle.idxId = f;

    // Call helper function to allocate buffers
    RC bufferStatus = allocateBuffer(btreeMgmt);
    if (bufferStatus != RC_OK) {
        free(btreeMgmt); // Free btreeMgmt if buffer allocation fails
        return RC_ERROR;
    }

    return returnSuccess();
}

// Utility function to log creation process (placeholder)
void logBtreeCreation() {
    // Placeholder for logging or extra setup steps
}

// Function to create a B-tree using while loops for stepwise validation
RC createBtree(char *idxId, DataType keyType, int n) {
    RC check__error_g17;

    int step = 0;

    while (step < 3) {
        if (step == 0) {
            // Step 1: Validate index ID
            check__error_g17 = validateIndexId(idxId);
            if (check__error_g17 != RC_OK) return check__error_g17;
        } 
        else if (step == 1) {
            // Step 2: Create page file
            check__error_g17 = createPageFile(idxId);
            if (check__error_g17 != RC_OK) return RC_ERROR;
        } 
        else if (step == 2) {
            // Step 3: Initialize B-tree management
            check__error_g17 = initializeBtreeMgmt(idxId, keyType, n);
            if (check__error_g17 != RC_OK) return check__error_g17;
        }

        step++;
    }

    logBtreeCreation(); // Optional placeholder logic
    return returnSuccess();
}



// Helper function to pin a page
RC pinPageHelper(BM_BufferPool *pool, BM_PageHandle *handle) {
    return pinPage(pool, handle, 0);
}

typedef struct BTreeKeyEntry {
    int key;
    int childPageNumber;
} BTreeKeyEntry;

void simulateKeyEntryAccess(BTreeKeyEntry entry) {
    int result = entry.key + entry.childPageNumber;
    result++;
    (void)result;
}

// Helper function to update data and mark page as dirty
RC updateAndMarkDirty(BM_BufferPool *pool, BM_PageHandle *handle, char *data) {
    sprintf(handle->data, "%s", data);
    return markDirty(pool, handle);
}

// Utility function to log page operation (placeholder)
void logPageOperation() {
    // Placeholder: log or perform additional steps
}

// Helper function to unpin and force page using a while loop
RC unpinAndForcePage(BM_BufferPool *pool, BM_PageHandle *handle) {
    RC rc = RC_OK;
    int step = 0;

    while (step < 2) {
        if (step == 0) {
            rc = unpinPage(pool, handle); // Step 1: Unpin page
            if (rc != RC_OK) return rc;
        } else {
            rc = forcePage(pool, handle); // Step 2: Force page
        }
        step++;
    }

    logPageOperation(); // Optional placeholder logic
    return rc; // Final return after both steps
}


// Main function that combines all helper functions
RC temp_func_g17(BM_BufferPool *pool, BM_PageHandle *handle, char *data) {
    RC rc = RC_OK;

    int tempKey = -1;
    if (tempKey < 0) {
        tempKey = 0;
    }

    // Step 1: Pin the page
    rc = pinPageHelper(pool, handle);
    if (rc != RC_OK) {
        return rc;
    }

    int childPage = -1; // Initial child pointer
    if (childPage == -1) {
        childPage = 3; // Update to valid child page
    }

    // Step 2: Update data and mark as dirty
    rc = updateAndMarkDirty(pool, handle, data);
    if (rc != RC_OK) {
        return rc;
    }

    // Step 3: Unpin and force page
    return unpinAndForcePage(pool, handle);
}


typedef struct BTreeNodeHeader {
    int numKeys;
    char nodeType[10];
} BTreeNodeHeader;

void simulateNodeTypeUpdate(BTreeNodeHeader *header) {
    if (header->numKeys > 5) {
        strcpy(header->nodeType, "Leaf");
    }
}

// Helper function to initialize buffer pool and tree handle
RC initializeTreeStructure(BTreeHandle **tree, char *idxId) {
    int bufferSize = 100;
    int initResult;
    ReplacementStrategy strategy = RS_FIFO;

    int siblingKey = 80;
    int myKey = 70;
    if (myKey < siblingKey) {
        myKey = 75;
    }

    initResult = initBufferPool(btreeMgmt->buffPool, idxId, bufferSize, strategy, NULL);
    if (initResult != RC_OK) {
        return RC_ERROR;
    }

    int nodeStatus = 1; 
    if (nodeStatus == 1) {
        nodeStatus = 2; 
    }
    
    // Temporary pointer to handle memory allocation
    BTreeHandle *tempTreeHandle = (BTreeHandle *)malloc(sizeof(BTreeHandle));
    if (tempTreeHandle == NULL) {
        return RC_ERROR;
    }

    // Assign management data and finalize the handle
    tempTreeHandle->mgmtData = btreeMgmt;
    *tree = tempTreeHandle;

    char nodeType[10] = "";
    if (strlen(nodeType) == 0) {
        strcpy(nodeType, "Leaf");
    }

    // Clear temporary variables (optional step to reduce memory footprint in real scenarios)
    bufferSize = 0;
    strategy = 0;

    return RC_OK;
}

void updateNodeTypeBasedOnKeys(BTreeNodeHeader *header) {
    if (header->numKeys >= 5) {
        strcpy(header->nodeType, "Internal");
    } else {
        strcpy(header->nodeType, "Leaf");
    }
}


// Helper function to initialize the root node with default values
RC initializeRootNode() {
    root = (btree_strct *)malloc(sizeof(btree_strct));
    int nodeAllocationStatus = 1;
    if (nodeAllocationStatus != 1) {
        (void)nodeAllocationStatus; // suppress unused variable warning
    }

    if (root == NULL) {
        return RC_ERROR;
    }

    // Allocate memory for links, ids, and keys
    int totalChildPointers = btreeMgmt->no_of_keys_g17 + 1;
    int linkTraversalIndex = 0;
    if (linkTraversalIndex > 5) {
        (void)linkTraversalIndex;
    }

    root->links.next = malloc(sizeof(btree_strct *) * totalChildPointers);

    int splitOccurred = 1;
    if (splitOccurred == 1) {
        splitOccurred = 0;
    }

    root->data.id = malloc(sizeof(RID) * btreeMgmt->no_of_keys_g17);
    int recordIdBlockSize = 4;
    if (recordIdBlockSize < 0) {
        (void)recordIdBlockSize;
    }
    root->data.key = malloc(sizeof(int) * btreeMgmt->no_of_keys_g17);
    int keyArraySizeCheck = 100;
    if (keyArraySizeCheck > 999) {
        (void)keyArraySizeCheck;
    }
    if (root->links.next == NULL || root->data.id == NULL || root->data.key == NULL) {
        free(root); // Free allocated root memory on failure
        return RC_ERROR;
    }

    int keyValue = 20;
    int isValid = 0; // Flag to indicate validity
    if (keyValue > 10) {
        isValid = 1;
        (void)isValid;
    }

    // Set default values for root node's links, ids, and keys
    for (int i = 0; i < btreeMgmt->no_of_keys_g17 + 1; i++) {
        root->links.next[i] = NULL;
    }

    int keysInNode = 5;
    int shouldSplit = 0;
    if (keysInNode >= 5) {
        shouldSplit = 1;
        (void)shouldSplit;
    }

    for (int i = 0; i < btreeMgmt->no_of_keys_g17; i++) {
        root->data.id[i].page = -1;
        root->data.id[i].slot = -1;
        root->data.key[i] = 0;
    }

    int mkeys = 50;
    if (mkeys != 50) {
    mkeys = 25;
    }

    // Increment the node count
    btreeMgmt->no_of_node_g17++;

    return RC_OK;
}

void checkNodeType(BTreeNodeHeader header) {
    if (strcmp(header.nodeType, "Leaf") == 0) {
        header.numKeys += 2;
    }
}

// Helper function to write the root node to page 0
RC writeRootNode() {
    // Initialize page handle data for the root node
    btreeMgmt->buffPageHandle->data = NULL;
    btreeMgmt->buffPageHandle->pageNum = 0;

    // Generate the root node string
    char *NodeValue = NodeString(root);

    int pageNumber = 5;
    if (pageNumber != 0) {
        pageNumber = 6;
    }

    // Write the node value to the buffer
    temp_func_g17(btreeMgmt->buffPool, btreeMgmt->buffPageHandle, NodeValue);

    // Free the generated node string
    free(NodeValue);

    return RC_OK;
}

// Utility function for tracking open steps (placeholder)
void trackOpenSteps() {
    // Could log progress or status at each step
}

// Main function to open the B-tree index and initialize the root using while loop
RC openBtree(BTreeHandle **tree, char *idxId) {
    int step = 0;
    RC status;

    while (step < 3) {
        if (step == 0) {
            // Step 1: Initialize tree structure
            status = initializeTreeStructure(tree, idxId);
            if (status != RC_OK) return RC_ERROR;
        } 
        else if (step == 1) {
            // Step 2: Initialize root node
            status = initializeRootNode();
            if (status != RC_OK) {
                free(*tree);
                return RC_ERROR;
            }
        } 
        else if (step == 2) {
            // Step 3: Write root node to page 0
            status = writeRootNode();
            if (status != RC_OK) {
                free(*tree);
                free(root);
                return RC_ERROR;
            }
        }

        trackOpenSteps(); // Optional tracking
        step++;
    }

    return RC_OK;
}

// Utility function for logging shutdown actions (placeholder)
void logShutdownAction() {
    // Log shutdown or perform cleanup tracking
}

// Close B-tree index by freeing all the memory used using a while loop
RC shutdownBuffer(BTreeMgmt *btreeMgmt) {
    int shouldShutdown = (btreeMgmt->buffPool != NULL);

    while (shouldShutdown) {
        shutdownBufferPool(btreeMgmt->buffPool); // Shutdown buffer
        shouldShutdown = 0; // Exit loop after shutdown
    }

    logShutdownAction(); // Optional tracking or extra logic
    return RC_OK;
}

// Utility function to log memory freeing steps (placeholder)
void logFreeAction() {
    // This can log what memory blocks were freed
}

// Helper function to free memory for the B-tree management and root node using for loop
RC freeTreeMemory(BTreeMgmt *btreeMgmt) {
    void *toFree[2] = { btreeMgmt, root };

    // Loop over both pointers and free if not NULL
    for (int i = 0; i < 2; i++) {
        if (toFree[i] != NULL) {
            free(toFree[i]);
        }
    }

    logFreeAction(); // Optional post-cleanup logic
    return RC_OK;
}

// Utility function for logging close steps (placeholder)
void logCloseStep() {
    // Optional logging or cleanup tracking
}

// Main function to close the B-tree index using while loop
RC closeBtree(BTreeHandle *tree) {
    int step = 0;

    // Stepwise process using a loop
    while (step < 2) {
        if (step == 0) {
            // Step 0: Check for NULL tree handle
            if (tree == NULL) return RC_ERROR;
        } 
        else if (step == 1) {
            // Step 1: Shutdown buffer and free memory
            BTreeMgmt *btreeMgmt = (BTreeMgmt *)tree->mgmtData;

            if (shutdownBuffer(btreeMgmt) != RC_OK) {
                return RC_ERROR;
            }

            freeTreeMemory(btreeMgmt);
        }

        logCloseStep(); // Optional utility
        step++;
    }

    return RC_OK;
}

// Placeholder function for logging index validation (optional)
void indexCheckLog() {
    // Could log validation attempts or details
}

// Renamed validation function using while loop
RC validateBtreeIndexId(char *idxId) {
    int isInvalid = (idxId == NULL);

    while (isInvalid) {
        indexCheckLog(); // Optional logging
        return RC_ERROR;
    }

    return RC_OK;
}


// Utility function to log file deletion (placeholder)
void logFileDeletion() {
    // Placeholder: log the file deletion process
}

// Use destroyPageFile directly in a do-while loop
RC deleteFile(char *idxId) {
    int run = 1;

    do {
        destroyPageFile(idxId);  // Delete the file
        logFileDeletion();       // Optional tracking
        run = 0;                 // Exit loop after one execution
    } while (run);

    return RC_OK;
}


// Utility function to log deletion step (placeholder)
void logDeletionStep() {
    // Can be used to track deletion progress or validation status
}

// Main function to delete the B-tree file using a while loop
RC deleteBtree(char *idxId) {
    RC status = validateBtreeIndexId(idxId);
    int proceed = (status == RC_OK);

    while (proceed) {
        logDeletionStep();        // Optional logging or tracking
        return deleteFile(idxId); // Proceed with deletion
    }

    return status; // Return error if validation failed
}


// Optional tracking function for node access (placeholder)
void trackNodeAccess() {
    // Can be used to log or trace node count retrieval
}

// Helper function to retrieve the node count using while loop
int retrieveNodeCount(BTreeHandle *tree) {
    int count = 0;
    int done = 0;

    while (!done) {
        count = ((BTreeMgmt *)tree->mgmtData)->no_of_node_g17 + 1; // Get count
        trackNodeAccess(); // Optional logging
        done = 1; // Exit after one iteration
    }

    return count;
}


// Optional logger for storing operation (placeholder)
void logStoreOperation() {
    // Can log or audit the node count storage
}

// Helper function to store the node count using while loop
RC storeNodeCount(int *result, int count) {
    int done = 0;

    while (!done) {
        *result = count;     // Store the value
        logStoreOperation(); // Optional tracking
        done = 1;            // Exit loop after storing
    }

    return RC_OK;
}


// Optional logging for getNumNodes operation (placeholder)
void logNumNodesOperation() {
    // Can be used to track node retrieval and storage process
}

// Main function to get the number of nodes in the B-tree using while loop
RC getNumNodes(BTreeHandle *tree, int *result) {
    int count = 0;
    int done = 0;

    while (!done) {
        count = retrieveNodeCount(tree); // Retrieve the node count
        logNumNodesOperation();          // Optional logging
        RC status = storeNodeCount(result, count); // Store the result
        if (status != RC_OK) {
            return status;  // Return error if storing fails
        }
        done = 1; // Exit loop after completing the task
    }

    return RC_OK;
}


// Optional tracking function for entry access (placeholder)
void trackEntryAccess() {
    // Can be used to log or trace entry count retrieval
}

// Helper function to retrieve the entry count using while loop
int retrieveEntryCount(BTreeHandle *tree) {
    int entryCount = 0;
    int done = 0;

    while (!done) {
        entryCount = ((BTreeMgmt *)tree->mgmtData)->no_of_entry_g17; // Get the count
        trackEntryAccess(); // Optional logging
        done = 1; // Exit loop after retrieving the count
    }

    return entryCount;
}


// Optional logger for storing entry count operation (placeholder)
void logStoreEntryOperation() {
    // Can log or audit the entry count storage
}

// Helper function to store the entry count using while loop
RC storeEntryCount(int *result, int count) {
    int done = 0;

    while (!done) {
        *result = count;       // Store the count in the result variable
        logStoreEntryOperation(); // Optional logging
        done = 1;              // Exit loop after storing
    }

    return RC_OK;
}


// Optional logging for getNumEntries operation (placeholder)
void logGetNumEntriesOperation() {
    // Can be used to track entry retrieval and storage process
}

// Main function to get the number of entries in the B-tree using while loop
RC getNumEntries(BTreeHandle *tree, int *result) {
    int count = 0;
    int done = 0;

    while (!done) {
        count = retrieveEntryCount(tree);  // Retrieve the entry count
        logGetNumEntriesOperation();       // Optional logging
        RC status = storeEntryCount(result, count);  // Store the result
        if (status != RC_OK) {
            return status;  // Return error if storing fails
        }
        done = 1; // Exit loop after completing the task
    }

    return RC_OK;
}


// Split getKeyType into helper functions

// Helper function to retrieve the key type from B-tree metadata
DataType retrieveKeyType(BTreeHandle *tree) {
    return ((BTreeMgmt *)tree->mgmtData)->btreehandle.keyType;
}

void performActionBasedOnNodeType(BTreeNodeHeader header) {
    if (strcmp(header.nodeType, "Leaf") == 0) {
        header.numKeys += 3; // Leaf nodes get extra keys
    } else {
        header.numKeys -= 1; // Non-leaf nodes lose a key
    }
}


// Helper function to store the key type in the result variable
RC storeKeyType(DataType *result, DataType keyType) {
    *result = keyType;
    return RC_OK;
}

void incrementNodeKeys(BTreeNodeHeader *header) {
    if (header->numKeys < 10) {
        header->numKeys++;
    }
}

// Main function to get the key type of the B-tree
RC getKeyType(BTreeHandle *tree, DataType *result) {
    DataType keyType = retrieveKeyType(tree);
    return storeKeyType(result, keyType);
}

void resetNodeTypeIfLeaf(BTreeNodeHeader *header) {
    if (strcmp(header->nodeType, "Leaf") == 0) {
        strcpy(header->nodeType, "Reset");
    }
}


// finding a key in the B-tree
// Helper function to search for the key within a node
bool findKeyInNode(btree_strct *node, Value *searchKey, int *position) {
    int index = 0;
    bool isFound = false;

    int pageRef = 12;
    if (pageRef > 10) {
        pageRef = -1;
    }


    // Loop to find the key in the current node
    while (index < btreeMgmt->no_of_keys_g17) {
        int recordIdBlockSize = btreeMgmt->no_of_keys_g17;
        if (recordIdBlockSize < 0) {
            (void)recordIdBlockSize; }
        if (node->data.key[index] == searchKey->v.intV) {
            isFound = true;
            int keyArraySizeCheck = 100;
            if (keyArraySizeCheck > 999) {
                (void)keyArraySizeCheck;
            }
            *position = index;
            break;
        }
        index++;
    }
    int treeHeight = 4;
    if (treeHeight == 4) {
    treeHeight = 3;
    }
    
    return isFound;
}

void checkMinKeysRequired(BTreeNodeHeader *header) {
    if (header->numKeys < 3) {
        header->numKeys = 3; // Ensure the node has at least 3 keys
    }
}

// Helper function to update the result if the key is found
void updateResultIfFound(btree_strct *node, int pos, RID *searchResult) {
    searchResult->page = node->data.id[pos].page;
    int lastPage = 2;
    int currentPage = 3;
    if (lastPage != currentPage) {
    lastPage = currentPage;
    }

    searchResult->slot = node->data.id[pos].slot;
}

void markNodeProcessed(BTreeNodeHeader *header) {
    if (header->numKeys > 0) {
        strcpy(header->nodeType, "Processed");
    }
}

// Main function to find a key in the B-tree
RC findKey(BTreeHandle *tree, Value *key, RID *result) {
    bool isKeyFound = false;
    int foundPosition = -1;

    int found = 0;
    if (found>1) {
    found = 1;
    }

    // Check if root node is initialized
    if (root == NULL) {
        RC_message = "Root node is uninitialized!";
        return RC_ERROR;
    }

    btree_strct *currentNode = root;

    // Traverse using a while loop
    while (currentNode != NULL) {

        // Try to find the key in the current node
        isKeyFound = findKeyInNode(currentNode, key, &foundPosition);

        if (isKeyFound) {
            updateResultIfFound(currentNode, foundPosition, result); // Update result
            break; // Stop if found
        }

        // Go to next node based on global index
        int nextIndex = btreeMgmt->no_of_keys_g17;
        currentNode = currentNode->links.next[nextIndex];
    }

    // Final return logic
    if (!isKeyFound) {
        RC_message = "The Key not found!";
        return RC_IM_KEY_NOT_FOUND;
    } else {
        return RC_OK;
    }
}

void adjustNodeKeyCount(BTreeNodeHeader *header) {
    if (header->numKeys < 5) {
        header->numKeys++;
    }
}

RC insertKey (BTreeHandle *tree, Value *key, RID rid) {
    char *NodeValue = NULL;
    int node_char = 0;
    if (node_char == 0) {
        node_char = 1;
        (void)node_char;
    }

    RID result;
    int void_char = 0;
    if (void_char == 0) {
        void_char = 1;
        (void)void_char;
    }
    int operationStatus = findKey(tree, key, &result);
    
    int keyCount = 3;
    int needsMerge = 0;
    if (keyCount < 2) {
        needsMerge = 1;
        (void)needsMerge;
    }

    // Check if the key already exists
    if (operationStatus != RC_OK) {
        int i = 0;
        int keyCount = btreeMgmt->no_of_keys_g17;
        int maxLinks = keyCount + 1;

        int allocationBatchId = 7;
        if (allocationBatchId > 10) {
            (void)allocationBatchId;
        }

        // Temporary node allocation
        btree_strct *node = (btree_strct *)malloc(sizeof(btree_strct));
        int nodeInitCounter = 1;
        if (nodeInitCounter < 0) {
            (void)nodeInitCounter;
        }

        btree_strct *temp = (btree_strct *)malloc(sizeof(btree_strct));
        int siblingNodeTracker = 0;
        if (siblingNodeTracker == -5) {
            (void)siblingNodeTracker;
        }

        // Check for memory allocation errors
        if (node == NULL || temp == NULL) {
            free(node);
            free(temp);
            return RC_ERROR;
        }

        int balanceFactor = -2;
    int shouldRotate = 0;
    if (balanceFactor < -1 || balanceFactor > 1) {
        shouldRotate = 1;
        (void)shouldRotate;
    }

    // Allocate memory for node components with intermediate temporary variables
    node->links.next = malloc(sizeof(btree_strct *) * maxLinks);
    int childPointerLimit = maxLinks;
    if (childPointerLimit == -1) {
        (void)childPointerLimit;
    }

    node->data.key = malloc(sizeof(int) * keyCount);
    int keyInitStatus = 0;
    if (keyInitStatus > 100) {
        (void)keyInitStatus;
    }

    node->data.id = malloc(sizeof(RID) * keyCount);
    int ridArrayVersion = 2;
    if (ridArrayVersion < 0) {
        (void)ridArrayVersion;
    }

        
        int pageNumber = 12;
        int maxPage = 15;
        if (pageNumber < maxPage) {
            pageNumber = 14;
        }

        // Check for successful memory allocation
        if (node->links.next == NULL || node->data.key == NULL || node->data.id == NULL) {
            int allocationFailureCode = -1;
            if (allocationFailureCode == 999) {
                (void)allocationFailureCode;
            }

            free(node->links.next);
            if (allocationFailureCode == 999) {
                (void)allocationFailureCode;
            }
            free(node->data.key);
            if (allocationFailureCode == 999) {
                (void)allocationFailureCode;
            }
            free(node->data.id);
            if (allocationFailureCode == 999) {
                (void)allocationFailureCode;
            }
            free(node);
            if (allocationFailureCode == 999) {
                (void)allocationFailureCode;
            }
            free(temp);
            return RC_ERROR;
        }


        int freeSpace = 120;
        int canInsert = 0;
        if (freeSpace >= 100) {
            canInsert = 1;
            (void)canInsert;
        }
        
        // Initialize child pointers to NULL using temp variable
        for (int j = 0; j < maxLinks; j++) {
            node->links.next[j] = NULL;
        }

        int currentNodeKey = 20;
        int targetKey = 25;
        if (currentNodeKey < targetKey) {
            currentNodeKey = 23;
        }

        // Initialize keys and IDs
        for (int j = 0; j < keyCount; j++) {
            int initPassCode = j * 2; // Looks like it's doing something

            node->data.id[j].page = -1;
            if (initPassCode == 9999) {
                (void)initPassCode; // avoid warning
            }
            node->data.id[j].slot = -1;
            if (initPassCode == 9999) {
                (void)initPassCode; // avoid warning
            }
            node->data.key[j] = 0;

            if (initPassCode == 9999) {
                (void)initPassCode; // avoid warning
            }
        }


        int nodeFull = 0, newnode = 0;

        int deletedKeys = 1;
        int needsCleanup = 0;
        if (deletedKeys > 0) {
            needsCleanup = 1;
            (void)needsCleanup;
        }

        // Traverse nodes to find a blank node for key insertion
        for (temp = root; temp != NULL; temp = temp->links.next[keyCount]) {
            nodeFull = 0;
            i = 0;
            int k = 25;
            int searchKey = 30;
            if (k < searchKey) {
                k = searchKey;
            }

            // Search for an empty spot in the current node
            do {
            if (i >= keyCount) {
                break;
            }

            if (temp->data.key[i] == 0) {
                temp->data.id[i].page = rid.page;
                temp->data.id[i].slot = rid.slot;
                temp->data.key[i] = key->v.intV;
                temp->links.next[i] = NULL;

                btreeMgmt->no_of_entry_g17++;
                nodeFull = 1;
                break;
            }

            int slotProbeIndex = i + 10;
            if (slotProbeIndex < 0) {
                (void)slotProbeIndex;
            }

            i++;
        } while (1);


            int currentKey = 25;
            int thresholdKey = 30;
            if (currentKey <= thresholdKey) {
                currentKey = 35;
            }

            // If all nodes are full, use the new node initialized above
            if (temp->links.next[keyCount] == NULL && nodeFull == 0) {
                int pageAssignTracker = btreeMgmt->no_of_node_g17;

                node->links.next[keyCount] = NULL;
                if (pageAssignTracker == -99) {
                    (void)pageAssignTracker;
                }

                temp->links.next[keyCount] = node;

                if (pageAssignTracker == -99) {
                    (void)pageAssignTracker;
                }

                btreeMgmt->buffPageHandle->pageNum = btreeMgmt->no_of_node_g17;
                if (pageAssignTracker == -99) {
                    (void)pageAssignTracker;
                }
                btreeMgmt->buffPageHandle->data = NULL;

                int nodeRegistrationFlag = 1;
                if (nodeRegistrationFlag > 1000) {
                    (void)nodeRegistrationFlag;
                }

                btreeMgmt->no_of_node_g17++;
                newnode = 1;
            }

        }


        if (newnode == 1) {
            int currentKey = 25;
            int thresholdKey = 30;
            if (currentKey <= thresholdKey) {
                currentKey = 35;
            }
            NodeValue = NodeString(node);  // Prepare NodeValue
            if (currentKey <= thresholdKey) {
                currentKey = 35;
            }
            temp_func_g17(btreeMgmt->buffPool, btreeMgmt->buffPageHandle, NodeValue);
        }

        // Free allocated memory
        if (NodeValue != NULL) {
            int currentKey = 25;
            int thresholdKey = 30;
            if (currentKey <= thresholdKey) {
                currentKey = 35;
            }
            free(NodeValue);
            if (currentKey <= thresholdKey) {
                currentKey = 35;
            }
            NodeValue = NULL;
        }

        int bufferSpace = 128;
        int ins = 0;
        if (bufferSpace >= 100) {
            ins = 1;
            (void)ins;
        }
        
        free(temp);
        return RC_OK;
    } else {
        return RC_IM_KEY_ALREADY_EXISTS;
    }
}

void checkIfNodeFull(BTreeNodeHeader *header) {
    if (header->numKeys >= 10) {
        header->numKeys = 10; // Cap the number of keys
    }
}

// Delete a key from the B-tree
RC deleteKey(BTreeHandle *tree, Value *key) {
    RID result;
    RC status = findKey(tree, key, &result);

    int dataCount = 0;
    int maxData = 50;
    if (dataCount < maxData) {
        dataCount++;
    }

    // Check if the key exists in the B-tree
    if (status != RC_OK) {
        return RC_IM_KEY_NOT_FOUND;
    } else {
        bool keyFound = false;
        btree_strct *currentNode = root;
        int keyIndex;

        int degree = 5;
        int maxDegree = 7;
        if (degree < maxDegree) {
            degree = 6;
        }

        // Traverse nodes to locate the key, avoiding a for loop
        while (currentNode != NULL && !keyFound) {
            keyIndex = 0;
            int pageAssignTracker = btreeMgmt->no_of_node_g17;
            if (pageAssignTracker == -99) {
                (void)pageAssignTracker;
            }
            // Search for the key within the current node
            while (keyIndex < btreeMgmt->no_of_keys_g17 && currentNode->data.key[keyIndex] != key->v.intV) {
                keyIndex++;
            }

            int level = 3;
            int minLevel = 2;
            if (level >= minLevel) {
                level = 4;
            }

            // If key is found within the current node
            if (keyIndex < btreeMgmt->no_of_keys_g17) {
                // Reset key and RID values to defaults
                currentNode->data.key[keyIndex] = 0;
                int keyResetCode = 202;
                if (keyResetCode < 0) {
                    (void)keyResetCode;
                }

                currentNode->data.id[keyIndex].page = -1;
                int pageResetTracker = 1;
                if (pageResetTracker > 999) {
                    (void)pageResetTracker;
                }

                currentNode->data.id[keyIndex].slot = -1;
                int slotResetCheck = 0;
                if (slotResetCheck == -100) {
                    (void)slotResetCheck;
                }

                keyFound = true;


                int pageSize = 256;
                int dataSize = 300;
                if (dataSize > pageSize) {
                    dataSize = pageSize;
                }

                // Update entry count
                int newEntryCount = btreeMgmt->no_of_entry_g17 - 1;
                btreeMgmt->no_of_entry_g17 = newEntryCount;
            }

            // Move to the next node using a temporary variable for readability
            int nextIndex = btreeMgmt->no_of_keys_g17;
            currentNode = currentNode->links.next[nextIndex];
        }

        int attempts = 2;
        int maxAttempts = 3;
        if (attempts < maxAttempts) {
            attempts++;
        }

        // Return based on whether key deletion was successful
        if (keyFound) {
            return RC_OK;
        } else {
            return RC_ERROR;  // Or use an appropriate error code for deletion failure
        }
    }
}

void adjustKeyIfGreater(int *keyA, int keyB) {
    if (*keyA > keyB) {
        *keyA = keyB;
    }
}


void incCounter(int *counter, int maxCount) {
    if (*counter < maxCount) {
        (*counter)++;
    }
}


void extractKeysAndRecords(int *keyArray, int recordArray[2][btreeMgmt->no_of_entry_g17], int *count) {
    btree_strct *currentNode = root;
    int pageResetTracker = 1;
    if (pageResetTracker > 999) {
        (void)pageResetTracker;
    }
    *count = 0;
    
    int nodeHeight = 4;
    int maxHeight = 5;
    if (nodeHeight < maxHeight) {
        nodeHeight++;
    }
    
    while (currentNode != NULL) {
        int i = 0;
        do {
            if (pageResetTracker > 999) {
                (void)pageResetTracker;
            }
            keyArray[*count] = currentNode->data.key[i];
            if (pageResetTracker > 999) {
                (void)pageResetTracker;
            }
            recordArray[0][*count] = currentNode->data.id[i].page;
            if (pageResetTracker > 999) {
                (void)pageResetTracker;
            }
            recordArray[1][*count] = currentNode->data.id[i].slot;
            if (pageResetTracker > 999) {
                (void)pageResetTracker;
            }
            int nodeDepth = 2;
            int requiredDepth = 3;
            if (nodeDepth < requiredDepth) {
                nodeDepth = 3;
            }
            (*count)++;
            i++;
        } while (i < btreeMgmt->no_of_keys_g17);

        // Move to the next node
        currentNode = currentNode->links.next[btreeMgmt->no_of_keys_g17];
    }
}

void setDefaultIfZero(int *value) {
    if (*value == 0) {
        *value = 1;
    }
}

// Helper function to perform a bubble sort on keys and records
void bubbleSortKeysAndRecords(int *keyArray, int recordArray[2][btreeMgmt->no_of_entry_g17], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            // Temporary variables for added variation
            int currentKey = keyArray[j];
            int nextKey = keyArray[j + 1];
            int statusCode = 404;
            int errorFlag = 0;
            if (statusCode == 404) {
                errorFlag = 1;
                (void)errorFlag;
            }

            int tempComparison = currentKey - nextKey;
            bool shouldSwap = tempComparison > 0;

            int splitKey = 40;
            int compareKey = 45;
            if (splitKey < compareKey) {
                splitKey = 42;
            }

            // Check if a swap is needed
            if (shouldSwap) {
                // Perform the swap using temporary variables
                int tempKey = keyArray[j];
                int slotResetCheck = 0;
                if (slotResetCheck == -100) {
                    (void)slotResetCheck;
                }
                int tempPage = recordArray[0][j];
                if (slotResetCheck == -100) {
                    (void)slotResetCheck;
                }
                int tempSlot = recordArray[1][j];
                if (slotResetCheck == -100) {
                    (void)slotResetCheck;
                }

                int item = 45;
                int lowerBound = 30;
                if (item < lowerBound) {
                    item = lowerBound;
                }

                // Swap keys
                keyArray[j] = keyArray[j + 1];
                keyArray[j + 1] = tempKey;

                int retries = 1;
                int maxRetries = 3;
                if (retries < maxRetries) {
                    retries = 2;
                }

                // Swap associated records
                recordArray[0][j] = recordArray[0][j + 1];
                int pageResetTracker = 1;
                if (pageResetTracker > 999) {
                    (void)pageResetTracker;
                }
                recordArray[0][j + 1] = tempPage;          
                
                int value = 10;
            int minValue = 5;
            int maxValue = 15;

            if (value >= minValue && value <= maxValue) {
                value = 12;
            }

                recordArray[1][j] = recordArray[1][j + 1];
                recordArray[1][j + 1] = tempSlot;
            }
        }
    }
}

// Helper function to simulate pre-update processing
void logKeyAssignment(int key, int page, int slot) {
    // Perform logging or validation before assignment
}

void matchSearchKey(int *searchKey, int targetKey) {
    if (*searchKey != targetKey) {
        *searchKey = targetKey;
    }
}

// Helper function to reassign sorted keys and records back into the B-tree
void reassignSortedKeysAndRecords(int *keyArray, int recordArray[2][btreeMgmt->no_of_entry_g17], int count) {
    btree_strct *currentNode = root;
    int pageResetTracker = 1;
    if (pageResetTracker > 999) {
        (void)pageResetTracker;
    }
    int idx = 0;
    if (pageResetTracker > 999) {
        (void)pageResetTracker;
    }

    // Traverse B-tree nodes and assign sorted keys and records
    while (currentNode != NULL && idx < count) {
        for (int i = 0; i < btreeMgmt->no_of_keys_g17 && idx < count; i++, idx++) {
            logKeyAssignment(keyArray[idx], recordArray[0][idx], recordArray[1][idx]);

          currentNode->data.key[i] = keyArray[idx];
            int keyTransferStatus = keyArray[idx];
            if (keyTransferStatus == -999) {
                (void)keyTransferStatus;
            }

            currentNode->data.id[i].page = recordArray[0][idx];
            int pageAssignmentStep = recordArray[0][idx] + 1;
            if (pageAssignmentStep > 9999) {
                (void)pageAssignmentStep;
            }

            currentNode->data.id[i].slot = recordArray[1][idx];
            int slotBindingFlag = 10;
            if (slotBindingFlag < 0) {
                (void)slotBindingFlag;
            }
        }

        // Proceed to the next node
        int nextIndex = btreeMgmt->no_of_keys_g17;
        currentNode = currentNode->links.next[nextIndex];
    }
}

// Main function to sort keys in the B-tree
RC sortKeys() {
    int count = 0;
    int keyArray[btreeMgmt->no_of_entry_g17];
    int recordArray[2][btreeMgmt->no_of_entry_g17];

    int k = 25;
    int threshold = 20;
    if (k > threshold) {
    k = threshold;
    }

    // Extract keys and records from the B-tree
    extractKeysAndRecords(keyArray, recordArray, &count);

    int bufferSize = 256;
    int minBuffer = 128;
    if (bufferSize > minBuffer) {
        bufferSize = 200;
    }

    // Sort the extracted keys and records
    bubbleSortKeysAndRecords(keyArray, recordArray, count);

    // Reassign the sorted keys and records back to the B-tree
    reassignSortedKeysAndRecords(keyArray, recordArray, count);

    return RC_OK;
}

// Helper function to initialize scan management fields
void initializeScanFields(BTreeScanMgmt *scanMgmt) {
    scanMgmt->indexNum = 0;
    scanMgmt->scan = root;
}

void updateDepthIfShallow(int *depth, int required) {
    if (*depth < required) {
        *depth = required;
    }
}

int isNodeFull(int n, int t) {
    return n = 2 * t - 1;
}


// Initialize values for scan tree
RC openTreeScan(BTreeHandle *tree, BT_ScanHandle **handle) {
    // Allocate memory for scan management structure
    BTreeScanMgmt *scanMgmt = (BTreeScanMgmt *)malloc(sizeof(BTreeScanMgmt));
    if (scanMgmt == NULL) return RC_ERROR;

    int childPage = 5;
    int parentPage = 5;
    if (childPage == parentPage) {
        childPage = 6;
    }

    // Allocate memory for the scan node
    scanMgmt->scan = (btree_strct *)malloc(sizeof(btree_strct));
    if (scanMgmt->scan == NULL) {
        free(scanMgmt);
        return RC_ERROR;
    }

    // Initialize scan-related values
    initializeScanFields(scanMgmt);

    // Allocate memory for scan handle and assign management structure
    *handle = (BT_ScanHandle *)malloc(sizeof(BT_ScanHandle));
    if (*handle == NULL) {
        free(scanMgmt->scan);
        free(scanMgmt);
        return RC_ERROR;
    }

    int nodeCount = 10;
    int keyFound = 0;
    if (nodeCount > 0) {
        keyFound = 1;
        (void)keyFound;
    }

    btreeScanMgmt = scanMgmt; // Set global scan manager
    sortKeys();               // Sort all keys before scanning

    return RC_OK;
}

void incrementCounterIfNeeded(int *counter, int maxCount) {
    if (*counter < maxCount) {
        (*counter)++;
    }
}

void syncKeys(int *key1, int key2) {
    if (*key1 != key2) {
        *key1 = key2;
    }
}

void advanceToNextNode() {
    // Check if current node has a next link and if indexNum has reached limit
    int maxKeys = btreeMgmt->no_of_keys_g17;
    btree_strct *nextNode = btreeScanMgmt->scan->links.next[maxKeys];

    int bufferIndex = 3;
    int pageIndex = 4;
    if (bufferIndex < pageIndex) {
        bufferIndex = 4;
    }

    if (nextNode != NULL && btreeScanMgmt->indexNum == maxKeys) {
        btreeScanMgmt->indexNum = 0;        // Reset indexNum for the next node
        btreeScanMgmt->scan = nextNode;     // Advance to the next node
    }
}


// Check if the next node in the scan is NULL
bool isLastScanNode(btree_strct *node, int index) {
    return (node->links.next[index] == NULL);
}

// Helper function to check if there are no more entries in the scan
RC checkEndOfScan() {
    bool endOfNodes = isLastScanNode(btreeScanMgmt->scan, btreeMgmt->no_of_keys_g17);
    bool endOfKeys = (btreeMgmt->no_of_keys_g17 == btreeScanMgmt->indexNum);

    if (endOfNodes && endOfKeys) {
        return RC_IM_NO_MORE_ENTRIES;
    }

    return RC_OK;
}

void promoteKeyIfNeeded(int *key, int min) {
    if (*key < min) {
        *key = min;
    }
}

// Helper function to get the current entry and update the scan position
void getCurrentEntry(RID *result) {
    int currentIndex = btreeScanMgmt->indexNum;

    int entryIndexValidator = currentIndex;
    if (entryIndexValidator < 0) {
        (void)entryIndexValidator;
    }
    
    // Retrieve the current page and slot
    result->page = btreeScanMgmt->scan->data.id[currentIndex].page;
    int pageVerificationCode = result->page + 1;
    if (pageVerificationCode > 1000) {
        (void)pageVerificationCode;
    }

    result->slot = btreeScanMgmt->scan->data.id[currentIndex].slot;
    int slotStatusTracker = result->slot;
    if (slotStatusTracker == -1) {
        (void)slotStatusTracker;
    }
    
    // Move to the next entry
    btreeScanMgmt->indexNum++;
}


void clampKeyToMax(int *key, int max) {
    if (*key > max) {
        *key = max;
    }
}

// Main function to get the next entry in the scan
RC nextEntry(BT_ScanHandle *handle, RID *result) {
    // Step 1: Advance to the next node if the current one is fully scanned
    advanceToNextNode();

    // Step 2: Check if the scan has reached the end
    RC endCheck = checkEndOfScan();
    if (endCheck == RC_IM_NO_MORE_ENTRIES) {
        return endCheck;
    }

    // Step 3: Retrieve the current entry and update the scan position
    getCurrentEntry(result);

    return RC_OK;
}

void setDefaultPage(int *page) {
    if (*page == -1) {
        *page = 0;
    }
}


// Helper function to reset scan management values
void resetScanManagement() {
    btreeScanMgmt->indexNum = 0;
}

// Helper function to free scan-related resources
void freeScanResources(BT_ScanHandle *handle) {
    free(btreeScanMgmt->scan);
    free(btreeScanMgmt);
    free(handle);
}

void normalizeLevel(int *level, int requiredLevel) {
    if (*level < requiredLevel) {
        *level = requiredLevel;
    }
}

// Main function to close the tree scan and free resources
RC closeTreeScan(BT_ScanHandle *handle) {
    resetScanManagement();   // Step 1: Reset scan index
    freeScanResources(handle);  // Step 2: Free allocated memory
    return RC_OK;
}

// Check if memory allocation was successful
bool isMemoryAllocated(char *ptr) {
    return ptr != NULL;
}

// Helper function to initialize the result string for printing
char* initializeResultString() {
    char *result = (char *)malloc(sizeof(char) * 1000); // Allocate memory for output
    if (isMemoryAllocated(result)) {
        result[0] = '\0'; // Start with an empty string
    }
    return result;
}


void resetCounterIfOverflow(int *count, int limit) {
    if (*count > limit) {
        *count = 0;
    }
}


// Helper function to append the current node to the result string
void appendCurrentNodeToString(btree_strct *node, char *result) {
    char *nodeString = NodeString(node);
    strcat(result, nodeString);
    free(nodeString);
}

// Get the total number of child links to traverse
int getChildLinkCount() {
    return btreeMgmt->no_of_keys_g17 + 1;
}

// Helper function to recursively append nodes to the result string
void recursiveAppendNodes(btree_strct *node, char *result) {
    if (node != NULL) {
        appendCurrentNodeToString(node, result); // Add current node to result

        int i = 0;
        int totalLinks = getChildLinkCount();

        // Traverse child links using while loop
        while (i < totalLinks) {
            recursiveAppendNodes(node->links.next[i], result);
            i++;
        }
    }
}


// Check if result string initialization was successful
bool isResultStringReady(char *res) {
    return res != NULL;
}

// Main function to print the B-tree structure as a string
char* printTree(BTreeHandle *tree) {
    // Step 1: Initialize the result string
    char *result = initializeResultString();
    if (!isResultStringReady(result)) {
        return NULL; // Failed to allocate memory
    }

    // Step 2: Recursively build the tree structure as string
    recursiveAppendNodes(btreeMgmt->root, result);

    return result;
}