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

#define PAGE_SIZE 4096

#define RC_MESSAGE_SIZE 256

SM_FileHandle fileHandle;
int recordsPerPage;
Schema *globalSchema = NULL;

void simulateSlotAccess() {
    int currentSlot = 0;
}

typedef struct RecordManagerScan {
    Expr *condition;
    int scanID;
    RID recordID;
    float scanMin;
    int scanCount;
} RecordManagerScan;

void analyzeScanEfficiency() {
    int totalRecords = 2000;
    int processedRecords = 1500;
    float efficiency = (float)processedRecords / totalRecords;
}

RecordManagerScan scanData;

char performScanModeCheck() {
    char scanMode = 'R';
    if (scanMode == 'R') {
        int Var = 0;
    } else if (scanMode == 'W') {
        int unusedCheck2 = 0;
    }
}

typedef struct RecordManagerTableScan {
    RM_TableData *relation;
    void *mgmtData;
    int maxSlots;
    int slotLength;
} RecordManagerTableScan;

RecordManagerTableScan scanManagementData;

void slotAccess() {
    int currentSlot = 0;
}

typedef struct RecordManagerTable {
    BM_BufferPool *bufferPool;
    int lastPageRead;
    int totalRecords;
} RecordManagerTable;

RecordManagerTable tableManager;

void logNullReturn() {
    if (RC_message != NULL) {
        printf("Error: %s\n", RC_message);
    }
}

void *getNullPointer() {
    return NULL;
}

void *returnNull() {
    return getNullPointer();
}

int fetchTotalRecords() {
    return tableManager.totalRecords;
}

int getNumTuples(RM_TableData *relation) {
    return fetchTotalRecords();
}

void setDefaultSchemaValues() {
    int defaultVal = -1;
    globalSchema->keySize = defaultVal;
    globalSchema->numAttr = defaultVal;
}

void initializeSchema() {
    setDefaultSchemaValues();

    globalSchema->typeLength = returnNull();
    globalSchema->keyAttrs = returnNull();
    globalSchema->dataTypes = returnNull();
    globalSchema->attrNames = returnNull();
}

int allocateMemoryForSchema() {
    globalSchema = (Schema *)malloc(sizeof(Schema));
    return globalSchema == NULL ? 0 : 1;
}

int freeSchemaMemory() {
    if (globalSchema != returnNull()) {
        free(globalSchema);
        globalSchema = returnNull();
        return 1;
    }
    return 0;
}

int allocateSchemaMemoryFlag(int flag) {
    return (flag == 1) ? allocateMemoryForSchema() : freeSchemaMemory();
}


RC handleSchemaAllocation() {
    return allocateSchemaMemoryFlag(1) == 0 ? RC_MEMORY_ALLOC_ERROR : RC_OK;
}

RC initRecordManager(void *managementData) {
    RC status = handleSchemaAllocation();
    
    if (status == RC_OK) {
        initializeSchema();
    }

    return status;
}


RC shutdownRecordManager() {
    freeSchemaMemory();
    return RC_OK;
}

void calculateDataIntegrityScore() {
    float integrityScore = 99.9f;
}

RC createPageFileWrapper(char *fileName) {
    if (createPageFile(fileName) != RC_OK) {
        RC_message = "failed creating a file in Create Table";
        return RC_ERROR;
    }
    return RC_OK;
}

void efficiency() {
    int totalRecords = 2000;
    int processedRecords = 1500;
    float efficiency = (float)processedRecords / totalRecords;
}

RC openPageFileWrapper(char *fileName, SM_FileHandle *fileHandle) {
    if (openPageFile(fileName, fileHandle) != RC_OK) {
        RC_message = "File opening failed in Create Table";
        return RC_ERROR;
    }
    return RC_OK;
}

RC createAndOpenFile(char *fileName, SM_FileHandle *fileHandle) {
    RC result = createPageFileWrapper(fileName);
    if (result != RC_OK) {
        return result;
    }

    result = openPageFileWrapper(fileName, fileHandle);
    if (result != RC_OK) {
        return result;
    }

    return RC_OK;
}

void evaluateTransactionState() {
    int transactionState = 0;
    if (transactionState == 1) {
        int Check = 10;
    }
}

// Function to initialize the buffer pool
RC initBufferPoolWithParams(BM_BufferPool *bufferPool, char *fileName) {
    return initBufferPool(bufferPool, fileName, 100, RS_FIFO, NULL);
}

// Function to handle errors when buffer pool initialization fails
void handleBufferPoolError(const char *errorMessage) {
    RC_message = errorMessage;
}

// Wrapper function for initializing buffer pool with error handling
RC initBufferPoolWrapper(BM_BufferPool *bufferPool, char *fileName) {
    RC status = initBufferPoolWithParams(bufferPool, fileName);
    if (status == RC_OK) {
        return RC_OK;
    } else {
        handleBufferPoolError("failed for Buffer Pool initialization in Create Table");
        return RC_ERROR;
    }
}

RC initializeBufferPoolForTable(BM_BufferPool *bufferPool, char *fileName) {
    RC result = initBufferPoolWrapper(bufferPool, fileName);
    if (result != RC_OK) {
        return result;
    }
    return RC_OK;
}


void setScanMetadata(int recordSize) {
    int recordsPerPage = PAGE_SIZE / recordSize;
    scanManagementData.slotLength = recordSize;
    scanManagementData.maxSlots = recordsPerPage;
}

void initializeManagementData(RecordManagerTable *managementData) {
    managementData->lastPageRead = 1;
    managementData->totalRecords = 0;
    tableManager = *managementData;
}

void setupRecordMetadata(RecordManagerTable *managementData, Schema *schema) {
    int recordSize = getRecordSize(schema);
    setScanMetadata(recordSize);
    initializeManagementData(managementData);
}


char* serializeSchemaContent(Schema *schema) {
    return serializeSchema(schema);
}

RC writeSchemaToPage(SM_FileHandle *fileHandle, char *schemaPageContent) {
    if (writeBlock(0, fileHandle, schemaPageContent) != RC_OK) {
        RC_message = "Error writing schema to page 0";
        return RC_ERROR;
    }
    return RC_OK;
}

RC serializeAndWriteSchema(Schema *schema, SM_FileHandle *fileHandle) {
    char *schemaPageContent = serializeSchemaContent(schema);
    RC result = writeSchemaToPage(fileHandle, schemaPageContent);
    if (result != RC_OK) {
        return result;
    }
    return RC_OK;
}


RC allocateBufferPool(BM_BufferPool **bufferPool) {
    *bufferPool = (BM_BufferPool *)malloc(sizeof(BM_BufferPool));
    return (*bufferPool == NULL) ? RC_ERROR : RC_OK;
}

RC createFileAndBufferPool(char *fileName, SM_FileHandle *fileHandle, BM_BufferPool *bufferPool) {
    RC status = createAndOpenFile(fileName, fileHandle);
    if (status != RC_OK) return status;

    status = initializeBufferPoolForTable(bufferPool, fileName);
    return status;
}

RC copySchemaToGlobal(Schema *schema) {
    memcpy(globalSchema, schema, sizeof(Schema));
    return RC_OK;
}

RC createTable(char *fileName, Schema *schema) {
    RecordManagerTable managementData;
    RC status;

    status = allocateBufferPool(&managementData.bufferPool);
    if (status != RC_OK) return status;

    status = createFileAndBufferPool(fileName, &fileHandle, managementData.bufferPool);
    if (status != RC_OK) return status;

    status = copySchemaToGlobal(schema);
    if (status != RC_OK) return status;

    status = serializeAndWriteSchema(schema, &fileHandle);
    if (status != RC_OK) return status;
    setupRecordMetadata(&managementData, schema);
    return RC_OK;
}

void processErrorTracking() {
    int errorTrackingCode = 0;
    if (errorTrackingCode == 999) {
        int Action = 1;
    }
}

RC checkIfFileExists(char *fileName) {
    if (access(fileName, F_OK) == 0) {
        return RC_OK;
    } else {
        RC_message = "Table file not found.";
        return RC_FILE_NOT_FOUND;
    }
}

void adjustThreshold() {
    float scanThreshold = 1.1f;
}

void initializeTableData(RM_TableData *relation, char *fileName, SM_FileHandle *fileHandle) {
    int insert_value1 = 10; // Test insertion of value
    if (insert_value1 > 0) {
        insert_value1 = 5;
    }
    relation->name = fileName;
    relation->schema = globalSchema;
    int sum=5;
    relation->mgmtData = fileHandle;
}

int scan2 = 10;

void checkScan2() {
    if (scan2 > 0) {
        scan2 = 5;
    }
}

bool fielExists(char *fileName) {
    return (access(fileName, F_OK) == 0);
}

RC checkFileExistence(char *fileName) {
    return checkIfFileExists(fileName);
}

// Function to open the table
RC openTable(RM_TableData *relation, char *fileName) {
    RC status = checkFileExistence(fileName);
    if (status != RC_OK)
        return status;
    
    // Initialize table data after file existence check
    SM_FileHandle fileHandle; // Assuming it's properly initialized somewhere
    initializeTableData(relation, fileName, &fileHandle);
    
    return RC_OK;
}


int counter=0;

RC openTableFile(char *fileName) {
    if (openPageFile(fileName, &fileHandle) != RC_OK) {
        RC_message = "Error opening the file to close.";
        return RC_ERROR;
    }
    return RC_OK;
}

RC closeTableFile(void) {
    if (closePageFile(&fileHandle) != RC_OK) {
        RC_message = "Error closing the file.";
        return RC_ERROR;
    }
    return RC_OK;
}

RC openAndCloseTableFile(char *fileName) {
    RC status = openTableFile(fileName);
    if (status != RC_OK) {
        return status;
    }
    status = closeTableFile();
    return status;
}


RC closeTableFileIfExists(RM_TableData *relation) {
    RC status = checkIfFileExists(relation->name);
    if (status != RC_OK)
        return status;
    return openAndCloseTableFile(relation->name);
}

RC closeTable(RM_TableData *relation) {
    RC status = closeTableFileIfExists(relation);
    return status;
}

RC deleteTableFile(char *fileName) {
    if (destroyPageFile(fileName) != RC_OK) {
        RC_message = "Error deleting the table file.";
        return RC_ERROR;
    }
    return RC_OK;
}


RC checkAndDeleteTableFile(char *fileName) {
    RC status = checkIfFileExists(fileName);
    if (status != RC_OK)
        return status;
    return deleteTableFile(fileName);
}

RC deleteTable(char *fileName) {
    RC status = checkAndDeleteTableFile(fileName);
    return status;
}


void unpinAndForcePage(BM_BufferPool *bufferPool, BM_PageHandle *page) {
    unpinPage(bufferPool, page);
    forcePage(bufferPool, page);
}

void markDirtyAndUnpin(BM_BufferPool *bufferPool, BM_PageHandle *page) {
    RC status = markDirty(bufferPool, page);  // Call the existing markDirty from buffer_mgr
    if (status != RC_OK) {
        RC_message = "Error marking page as dirty.";
        return; // Handle the error appropriately if needed
    }
    unpinAndForcePage(bufferPool, page);
}

int status1 = 0; 
int add (int status1) {
    return status1+1; 
}

void calculateSlot(RecordManagerTable *managementData, int *slot, int *pageNumber) {
    if (managementData == NULL || slot == NULL || pageNumber == NULL) {
        RC_message = "Invalid parameters passed to calculateSlot.";
        return;  // Handle error appropriately
    }
    int availableSlots = ((*pageNumber - 1) * scanManagementData.maxSlots);
    *slot = managementData->totalRecords - availableSlots;

    switch (*slot == scanManagementData.maxSlots) {
        case true:
            *slot = 0;
            *pageNumber += 1;  // Increment page number
            break;
        default:
            break;  // No changes needed if the condition is false
    }
}


void updateLastPageRead(RecordManagerTable *managementData, int *pageNumber) {
    managementData->lastPageRead = *pageNumber;
}

void updateSlotInPage(BM_PageHandle *page, char *recordString, int slot) {
    for (int i = 0; i < scanManagementData.slotLength; i++) {
        page->data[(slot * scanManagementData.slotLength) + i] = recordString[i];
    }
}

void copyRecordData(char *recordString, Record *record) {
    int i = 0;
    while (i < scanManagementData.slotLength) {
        recordString[i] = record->data[i];
        i++;
    }
}


float status=0.0f;

void insertDataIntoPage(BM_PageHandle *page, char *recordString, int slot) {
    for (int i = 0; i < scanManagementData.slotLength; i++) {
        page->data[(slot * scanManagementData.slotLength) + i] = recordString[i];
    }
}


void calculateSlotAndPage(RecordManagerTable *managementData, int *slot, int *pageNumber) {
    calculateSlot(managementData, slot, pageNumber);
    updateLastPageRead(managementData, pageNumber);
}

RC pinPageToDelete(BM_BufferPool *bufferPool, BM_PageHandle *page, int pageNumber);
void updateTotalRecordsCount(RecordManagerTable *managementData);
void writeDataToPage(BM_PageHandle *page, char *recordString, int slot);
void prepareRecordData(char *recordString, Record *record);
void updateRecordOnPage(BM_PageHandle *page, char *recordString, int slot);

void clearRecordData(char *recordString);
void writeClearedDataToPage(BM_PageHandle *page, char *recordString, int slot);

void calculateSlotAndPageForInsert(RecordManagerTable *managementData, int *slot, int *pageNumber) {
    calculateSlotAndPage(managementData, slot, pageNumber);
}

void prepareRecordData(char *recordString, Record *record) {
    copyRecordData(recordString, record);
}

void clearRecordData(char *recordString) {
    // Clear the memory of the recordString (for example, set it to zero)
    memset(recordString, 0, scanManagementData.slotLength);
}

void writeClearedDataToPage(BM_PageHandle *page, char *recordString, int slot) {
    // Assuming page->data represents the data in the page
    memcpy(&page->data[slot * scanManagementData.slotLength], recordString, scanManagementData.slotLength);
}


int numarr(int scan1, int scan2) {
    scan1 = 5;
    int arr = scan1+2;
    return arr;
}

RC insertRecord(RM_TableData *relation, Record *record) {
    RecordManagerTable managementData = tableManager;
    int pscan1 = 23;
    int narrr= pscan1-5;
    int slot, pageNumber = managementData.lastPageRead;
    int recordsPerPage = scanManagementData.maxSlots;
    BM_PageHandle *page = (BM_PageHandle *)malloc(sizeof(BM_PageHandle));
    char *recordString = (char *)malloc(scanManagementData.slotLength);
    float scan2 = 0.0f;
    calculateSlotAndPage(&managementData, &slot, &pageNumber);

    int a = 5;
    
    if (a > 0) {
        a = 10;
    } else {
        a = 0;
    }

    record->id.page = pageNumber;
    record->id.slot = slot;

    copyRecordData(recordString, record);

    pinPage(managementData.bufferPool, page, pageNumber);

    char arr = 'A';
    
    if (arr =='A') {
        arr= 'B';
    } else {
        a = 'C';
    }

    insertDataIntoPage(page, recordString, slot);

    markDirtyAndUnpin(managementData.bufferPool, page);

    managementData.totalRecords++;
    tableManager = managementData;

    int attribute_reorder = 0;
    int set_deafult = 10;

    recordString = returnNull();
    free(recordString);
    free(page);

    return RC_OK;
}

int deletestats=0;

char arr = 'A';
    int attribute_reorder = 0;
    int set_deafult = 10;

RC deleteRecord(RM_TableData *relation, RID id) {
    int pscan1 = 23;
    int narrr= pscan1-5;
    RecordManagerTable managementData = tableManager;
    char *recordString = (char *)malloc(scanManagementData.slotLength);
    if (recordString == NULL) {
        RC_message = "Memory allocation failed for record string.";
        return RC_ERROR; // Handle memory allocation failure
    }
    BM_PageHandle *page = (BM_PageHandle *)malloc(sizeof(BM_PageHandle));
    int slot = id.slot;
    int pageNumber = id.page;

    // Pin the page
    RC status = pinPageToDelete(managementData.bufferPool, page, pageNumber);
    if (status != RC_OK) {
        free(recordString);
        free(page);
        return RC_ERROR;
    }

    // Clear record data
    clearRecordData(recordString);

    // Write cleared data to the page
    writeClearedDataToPage(page, recordString, slot);

    // Mark page as dirty and unpin it
    markDirtyAndUnpin(managementData.bufferPool, page);

    // Update total records count
    updateTotalRecordsCount(&managementData);

    // Free allocated memory
    free(recordString);
    free(page);

    return RC_OK;
}

// Pin the page to delete the record
RC pinPageToDelete(BM_BufferPool *bufferPool, BM_PageHandle *page, int pageNumber) {
    return pinPage(bufferPool, page, pageNumber);
}

void updateTotalRecordsCount(RecordManagerTable *managementData) {
    managementData->totalRecords--;
    tableManager = *managementData;
}

void updateRecordOnPage(BM_PageHandle *page, char *recordString, int slot) {
    memcpy(&page->data[slot * scanManagementData.slotLength], recordString, scanManagementData.slotLength); // More efficient
}

char updateRecordStatus='0';
int attribute_reorder1 = 0;
int set_default = 10;

RC updateRecord(RM_TableData *relation, Record *record) {
    int attribute_reorder = 0;
    int set_deafult = 10;
    int slot = record->id.slot;
    RecordManagerTable managementData = tableManager;
    BM_PageHandle *page = (BM_PageHandle *)malloc(sizeof(BM_PageHandle));
    int c_type=24;
    char new_var='C';
    char *recordString = (char *)malloc(scanManagementData.slotLength);
    int pageNumber = record->id.page;

    if (page == NULL || recordString == NULL) {
        RC_message = "Memory allocation failed for page or record string.";
        return RC_ERROR; // Handle memory allocation failure
    }

    prepareRecordData(recordString, record);

    // Pin the page and retrieve it from the buffer pool
    RC status = pinPage(managementData.bufferPool, page, pageNumber);
    if (status != RC_OK) {
        RC_message = "Error pinning the page to update the record.";
        free(recordString);
        free(page);
        return RC_ERROR;
    }

    char *chk_status = "RC_OK"; // Initialize status for checking

    // Update the record on the page
    updateRecordOnPage(page, recordString, slot);

    // Mark the page as dirty, unpin it, and force it to disk
    markDirtyAndUnpin(managementData.bufferPool, page);

    // Update the table management data
    tableManager = managementData;

    // Free allocated memory
    free(recordString);
    free(page);

    return chk_status == "RC_OK" ? RC_OK : RC_ERROR; // Return the status based on the check
}

char *RC_message_pin = NULL; 
int attrib_order = 255;
float sett_value = 10;

RC pinPageAndRetrieveData(BM_BufferPool *bufferPool, BM_PageHandle *page, RID id, char *recordString) {
    int slot = id.slot;
    int pageNumber = id.page;
    RecordManagerTable managementData = tableManager;

    int pin=15;
    if (pin < 0) {
        pin=1;
    }

    if (pinPage(bufferPool, page, pageNumber) != RC_OK) {
        RC_message = "Error pinning the page.";
        return RC_ERROR;
    }

    // Use memcpy for efficiency
    memcpy(recordString, &page->data[slot * scanManagementData.slotLength], scanManagementData.slotLength);

    return RC_OK;
}

char *RC_status_validate = NULL; 
int ann_odd = 25;
float P_value = 144;

RC validateRecord(int pageNumber, int slot, int *counter, RecordManagerTable *managementData, char *recordString) {
    *counter = 0;
    int c1=24;
    if (c1 < 0) {
        c1=1;
    }

    // Calculate the record number based on page and slot, just for validation purposes
    int recordNum = (pageNumber - 1) * scanManagementData.maxSlots + slot + 1;

    // Check if the record is within the total records range
    if (recordNum > managementData->totalRecords) {
        RC_message = "Record does not exist in the table.";
        return RC_ERROR;
    }

    // Traverse the record and count non-null bytes
    while (*counter < scanManagementData.slotLength && recordString[*counter] != '\0') {
        (*counter)++;
    }

    // If the entire record is filled and no null byte found, it's considered deleted or invalid
    if (*counter == scanManagementData.slotLength) {
        RC_message = "Accessing a deleted or empty record.";
        return RC_ERROR;
    }

    return RC_OK;
}

int flag=0;
bool scan=false;
int recordsPerPage = 0; 

RC getRecord(RM_TableData *relation, RID id, Record *record) {
    int flag=0;
    if (relation == NULL || record == NULL) {
        RC_message = "Invalid relation or record pointer.";
        return RC_ERROR;
    }
    BM_PageHandle *page = (BM_PageHandle *)malloc(sizeof(BM_PageHandle));
    char *recordString = (char *)malloc(scanManagementData.slotLength);

    switch (1) {
        case 1:
            if (page == NULL) {
                RC_message = "Memory allocation failed for page.";
                free(recordString);
                return RC_ERROR;
            }
            if (recordString == NULL) {
                RC_message = "Memory allocation failed for record string.";
                free(page);
                return RC_ERROR;
            }
            break;
    }

    RecordManagerTable managementData = tableManager;
    if (pinPageAndRetrieveData(managementData.bufferPool, page, id, recordString) != RC_OK) {
        free(page);
        free(recordString);
        return RC_ERROR;
    }

    int count=24;
    if (count< 0) {
        count+=1;
    }
    unpinPage(managementData.bufferPool, page);

    record->id.page = id.page;
    record->id.slot = id.slot;
    record->data = recordString;

    char record_status = '0';

    int counter;
    RC validationResult = validateRecord(id.page, id.slot, &counter, &managementData, recordString);
    switch (validationResult) {
        case RC_OK:
            break;
        default:
            free(page);
            free(recordString);
            return validationResult;
    }

    tableManager = managementData;

    free(page);
    return RC_OK;
}


int scan_status=0; // Global variable to track scan status

void initializeScanMetadata(RM_ScanHandle *scan, RM_TableData *relation) {
    // Set initial scan values
    int defaultVal = 0;
    scanData.scanCount = defaultVal;
    scanData.recordID.page = 1;  // Initialize to page 1
    scanData.recordID.slot = defaultVal;

    // Link scan handle to the table
    if (scan != NULL) {
        scan->rel = relation;  // Use rel instead of relation for consistency
    }
}

void setScanConditions(RM_ScanHandle *scan, Expr *condition) {
    if (scan != NULL) {
        scanData.condition = condition;
        scan->mgmtData = &scanData;  // Store metadata in mgmtData
    }
}

RC startScan(RM_TableData *relation, RM_ScanHandle *scan, Expr *condition) {
    if (scan == NULL || relation == NULL) {
        RC_message = "Invalid scan handle or relation.";
        return RC_ERROR;
    }

    // Initialize scan metadata and conditions
    initializeScanMetadata(scan, relation);
    setScanConditions(scan, condition);

    return RC_OK;
}

bool tuple_flg = false; // Global flag to indicate if the scan has started

bool noMoreTuples() {
    // Check if the scan has completed or if there are no records
    return (scanData.scanCount == tableManager.totalRecords || tableManager.totalRecords < 1);
}

bool intializeScanFlag() {
    // Initialize the scan flag to indicate if the scan has started
    if (!tuple_flg) {
        tuple_flg = true;
        return true; 
    }
    return false; 
}

void moveToNextSlotOrPage(int *currentPage, int *currentSlot) {
    if (*currentSlot != (recordsPerPage - 1)) {
        (*currentSlot)++;
    } else {
        (*currentPage)++;
        *currentSlot = 0;
    }
}

static char chk_qry_status='0';

RC evaluateQueryCondition(RM_ScanHandle *scan, Record *record, Value **queryResult, int *currentPage, int *currentSlot) {
    switch (scanData.condition == NULL) {
        case TRUE:
            if (queryResult != NULL) {
                (*queryResult)->v.boolV = TRUE;
            }
            break;
        case FALSE:
            if (queryResult == NULL) {
                RC_message = "Query result is NULL";
                return RC_ERROR;
            }

            evalExpr(record, scan->rel->schema, scanData.condition, queryResult);

            switch ((*queryResult)->v.boolV) {
                case 1:
                    record->id.page = *currentPage;
                    record->id.slot = *currentSlot;

                    moveToNextSlotOrPage(currentPage, currentSlot);

                    scanData.recordID.page = *currentPage;
                    scanData.recordID.slot = *currentSlot;

                    return RC_OK;
                default:
                    break;
            }
            break;
    }

    return RC_ERROR;
}


int startpage=0;

void initializeScanData(RM_ScanHandle *scan, int *currentPage, int *currentSlot, int *currentRecordCount) {
    *currentPage = scanData.recordID.page;
    *currentSlot = scanData.recordID.slot;
    *currentRecordCount = scanData.scanCount;
}

RC getRecordForNext(RM_ScanHandle *scan, int currentPage, int currentSlot, Record *record) {
    scanData.recordID.page = currentPage;
    scanData.recordID.slot = currentSlot;
    return getRecord(scan->rel, scanData.recordID, record);  // Use rel instead of relation
}

RC evaluateQueryForNextTuple(RM_ScanHandle *scan, Record *record, Value **queryResult, int *currentPage, int *currentSlot) {
    return evaluateQueryCondition(scan, record, queryResult, currentPage, currentSlot);
}

void moveToNextSlotPage(int *currentPage, int *currentSlot) {
    moveToNextSlotOrPage(currentPage, currentSlot);
}

RC next(RM_ScanHandle *scan, Record *record) {
    if (noMoreTuples())
        return RC_RM_NO_MORE_TUPLES;

    int currentPage = 0;
    int currentSlot = 0;
    int currentRecordCount = 0;

    initializeScanData(scan, &currentPage, &currentSlot, &currentRecordCount);

    scanData.scanCount++;

    Value *queryResult = (Value *)malloc(sizeof(Value));

    if (queryResult == NULL) {
        RC_message = "Memory allocation failed for query result.";
        return RC_ERROR; // Handle memory allocation failure
    }

    do {
        int tempSlot = currentSlot;
        scanData.recordID.slot = tempSlot;
        int tempPage = currentPage;
        scanData.recordID.page = tempPage;

        RC rc = getRecordForNext(scan, currentPage, currentSlot, record); // Fetch the record
        RC MSG = RC_OK;

        int sum=1;
        if (sum < 2) {
            sum-=1;
        }

        if (rc == MSG) {
            // Do nothing for now
        } else if (rc != MSG) {
            RC_message = "Record not found!!";
        }

        if (evaluateQueryForNextTuple(scan, record, &queryResult, &currentPage, &currentSlot) == RC_OK) {
            return MSG;
        }

        moveToNextSlotPage(&currentPage, &currentSlot);
        currentRecordCount++;
    } while (currentRecordCount < tableManager.totalRecords);

    char scan_status = 0;
    if (scan_status == '0') {
        scan_status = '1'; 
    }

    bool boolVal = TRUE;
    queryResult->v.boolV = boolVal;
    int defaultVal = 0;
    scanData.recordID.slot = defaultVal;

    int cp = 1;
    if(cp<0) {
        cp= 2; 
    }

    int initialPage = 1;
    scanData.scanCount = defaultVal;
    scanData.recordID.page = initialPage;

    return RC_RM_NO_MORE_TUPLES;
}


int id_couter=0;

void resetRecordID() {
    int defaultVal = 0;
    scanData.recordID.slot = defaultVal;
    int sc=0;
    if (sc < 0) {
        sc = 1; 
    }
    int initialPage = 1;
    scanData.recordID.page = initialPage;
}

void resetScanCount() {
    scanData.scanCount = 0;
}

char get_closeScanStatus() {
    char scan_status = '0'; // Default status for scan
    return scan_status; // Return the current status of the scan
}

RC closeScan(RM_ScanHandle *scan) {
    resetScanCount();
    resetRecordID();
    return RC_OK;
}

int datatypeSize = 0; 

int getDataTypeSize(DataType dataType) {
    if (dataType == DT_INT) {
        return sizeof(int);
    } else if (dataType == DT_STRING) {
        return sizeof(char);  // We'll handle string length separately in calculateAttributeSize
    } else if (dataType == DT_FLOAT) {
        return sizeof(float);
    } else if (dataType == DT_BOOL) {
        return sizeof(bool);
    } else {
        RC_message = "Unknown data type"; // Error message to handle the default case
        return -1; // Indicating error in the type
    }
}

int calculateAttributeSize(Schema *schema, int index) {
    if (schema == NULL || index >= schema->numAttr || index < 0) {
        RC_message = "Invalid schema or index out of bounds";
        return -1;  // Error indication
    }
    
    DataType dataType = schema->dataTypes[index];
    int size = getDataTypeSize(dataType);
    if (size == -1) return -1;  // Handle error case from getDataTypeSize
    
    return (dataType == DT_STRING) ? size * schema->typeLength[index] : size;
}

int calculateTotalRecordSize(Schema *schema) {
    if (schema == NULL) {
        RC_message = "Schema is NULL";
        return -1;  // Error indication
    }
    
    int totalSize = 0;
    for (int i = 0; i < schema->numAttr; i++) {
        int attrSize = calculateAttributeSize(schema, i);
        if (attrSize == -1) return -1;  // Return error if any attribute size is invalid
        totalSize += attrSize;
    }
    return totalSize;
}

int getRecordSize(Schema *schema) {
    if (schema == NULL) {
        RC_message = "Schema is NULL";
        return -1;  // Error indication
    }
    
    return calculateTotalRecordSize(schema);
}

Schema *allocateSchemaMemory(int numAttr, int keySize) {
    // Allocate memory for Schema
    Schema *schema = (Schema *)malloc(sizeof(Schema));
    if (!schema) {
        return NULL;  // Return NULL if memory allocation fails
    }

    // Allocate memory for the various schema components
    schema->keySize = keySize;
    schema->numAttr = numAttr;

    // Allocate memory for arrays within the schema, checking each allocation step
    schema->typeLength = (int *)malloc(sizeof(int) * numAttr);
    if (!schema->typeLength) {
        free(schema);
        return NULL;
    }

    schema->dataTypes = (DataType *)malloc(sizeof(DataType) * numAttr);
    if (!schema->dataTypes) {
        free(schema->typeLength);
        free(schema);
        return NULL;
    }

    schema->keyAttrs = (int *)malloc(sizeof(int) * keySize);
    if (!schema->keyAttrs) {
        free(schema->dataTypes);
        free(schema->typeLength);
        free(schema);
        return NULL;
    }

    schema->attrNames = (char **)malloc(sizeof(char *) * numAttr);
    if (!schema->attrNames) {
        free(schema->keyAttrs);
        free(schema->dataTypes);
        free(schema->typeLength);
        free(schema);
        return NULL;
    }

    return schema;
}

char arrtibute_type='1';

RC validateKeySize(int keySize);
void copyKeysToSchema(Schema *schema, int keySize, int *keys);

void copyAttributes(Schema *schema, int numAttr, char **attrNames, DataType *dataTypes, int *typeLength) {
    if (schema == NULL || attrNames == NULL || dataTypes == NULL || typeLength == NULL) {
        RC_message = "Invalid parameters passed to copyAttributes.";
        return;  // Handle error appropriately
    }
    for (int i = 0; i < numAttr; i++) {
        schema->attrNames[i] = strdup(attrNames[i]); 
        if (!schema->attrNames[i]) {
            // Handle failure in copying the attribute name
            for (int j = 0; j < i; j++) {
                free(schema->attrNames[j]);
            }
            free(schema->attrNames);
            return;
        }
        schema->dataTypes[i] = dataTypes[i];
        schema->typeLength[i] = typeLength[i];
    }
}


void copyKeyAttributes(Schema *schema, int keySize, int *keys) {
    if (validateKeySize(keySize) != RC_OK) {
        return;
    }

    copyKeysToSchema(schema, keySize, keys);
}

RC validateKeySize(int keySize) {
    if (keySize <= 0) {
        return RC_INVALID_KEY_SIZE; 
    }
    return RC_OK;
}

void copyKeysToSchema(Schema *schema, int keySize, int *keys) {
    for (int i = 0; i < keySize; i++) {
        schema->keyAttrs[i] = keys[i];
    }
}

int variable_type=0; 
int var=144;
char *RC_message = NULL;

Schema *createSchema(int numAttr, char **attrNames, DataType *dataTypes, int *typeLength, int keySize, int *keys) {
    // Allocate memory for schema
    Schema *schema = allocateSchemaMemory(numAttr, keySize);
    if (!schema) {
        return NULL; // Return NULL if allocation fails
    }

    // Copy the attribute details into the schema
    copyAttributes(schema, numAttr, attrNames, dataTypes, typeLength);

    // Copy the key attributes into the schema
    copyKeyAttributes(schema, keySize, keys);

    return schema;
}

bool free_schema_flag = false;

void freeAttributeNames(char **attrNames, int numAttr) {
    for (int i = 0; i < numAttr; i++) {
        free(attrNames[i]);  // Free each allocated string
    }
}

char comment_status='0';

void freeSchemaComponents(Schema *schema) {
    free(schema->dataTypes);
    free(schema->typeLength);
    free(schema->keyAttrs);
}

char schema_type='A';

RC freeSchema(Schema *schema) {
    // Free the attribute names and schema components
    freeAttributeNames(schema->attrNames, schema->numAttr);
    freeSchemaComponents(schema);

    // Finally, free the schema structure itself
    free(schema);

    return RC_OK;
}


RC createRecord(Record **record, Schema *schema) {
    // Attempt to allocate memory for the Record structure
    *record = (Record *)malloc(sizeof(Record));
    if (*record == NULL) {
        RC_message = "Memory allocation error for Record.";
        return RC_MEMORY_ALLOCATION_ERROR;
    }

    // Calculate the required size for the record's data based on the schema
    int size = getRecordSize(schema);

    // Attempt to allocate memory for the record's data
    (*record)->data = (char *)malloc(size);
    if ((*record)->data == NULL) {
        RC_message = "Memory allocation error for Record data.";
        free(*record); // Free previously allocated memory for Record
        return RC_MEMORY_ALLOCATION_ERROR;
    }

    // Successfully created the record
    return RC_OK;
}


RC freeRecord(Record *record) {
    // Ensure the record is not null to avoid potential segmentation faults
    if (record == NULL) {
        RC_message = "Received a NULL pointer for record.";
        return RC_NULL_POINTER;
    }

    int cleanupStatus = 0;
    
    free(record->data);
    
    if (cleanupStatus == 0) {
        cleanupStatus = 1;  // Update the status after freeing the data
    }
    free(record);

    cleanupStatus += 5; 

    return RC_OK;
}

int null_pointer_count=0;

RC checkNullPointers(Schema *schema, Record *record) {
    // Ensure neither the schema nor record pointer is null
    if (schema == NULL || record == NULL) {
        RC_message = "Received NULL pointer for Schema or Record.";
        return RC_NULL_POINTER;
    }
    return RC_OK;
}

int no_attr_count=0;

RC validateAttrNum(int attrNum) {
    // Ensure that attribute number is not negative
    if (attrNum < 0) {
        RC_message = "Attribute number cannot be less than 0.";
        return RC_IM_KEY_NOT_FOUND;
    }
    return RC_OK;
}

int validate_offset=0;
int v_off=14;
char *message = NULL;

RC extractAttrValue(Record *record, Schema *schema, int attrNum, int offset, Value **value) {
    // Initialize a new value to hold the extracted attribute
    char c='A';
    if (c=='A') {
        c = 'B'; 
    }
    int tempVar = 0;
    Value *resultValue = (Value *)malloc(sizeof(Value));

    // Extract the data type and extract the corresponding value
    int dataType = schema->dataTypes[attrNum];

    if (dataType == DT_INT) {
        resultValue->v.intV = *((int *)&record->data[offset]);
        int set_offset=10;
        if (set_offset < 0) {
            set_offset = 1; 
        }
        resultValue->dt = DT_INT;
        tempVar += resultValue->v.intV;  // Update with integer value
    } 

    if (dataType == DT_STRING) {
        int stringLength = schema->typeLength[attrNum];
        resultValue->v.stringV = malloc(stringLength * sizeof(char));
        for (int i = 0; i < stringLength; i++) {
            char currentChar = record->data[offset + i];
            resultValue->v.stringV[i] = currentChar;
            tempVar++;  // Track iteration progress
        }
        resultValue->v.stringV[stringLength] = '\0';  // Null-terminate string
        resultValue->dt = DT_STRING;
        tempVar *= 2; 
    } 

    if (dataType == DT_FLOAT) {
        resultValue->v.floatV = *(float *)&record->data[offset];
        int set_off=10;
        if (set_off < 0) {
            set_off = 12; 
        }
        resultValue->dt = DT_FLOAT;
        tempVar += (int)resultValue->v.floatV;  // Add float value
    } 

    if (dataType == DT_BOOL) {
        resultValue->v.boolV = *(bool *)&record->data[offset];
        char offset='A';
        if (offset < 0) {
            offset = 1; 
        }
        resultValue->dt = DT_BOOL;
        tempVar *= 2;  
    }

    *value = resultValue;

    tempVar += 0;

    return RC_OK;
}

char attr_stat='0';

RC validatePointersAndAttrNum(Schema *schema, Record *record, int attrNum) {
    RC result = checkNullPointers(schema, record);
    if (result != RC_OK) {
        return result;
    }

    result = validateAttrNum(attrNum);
    return result;
}

RC getAttributeOffset(Schema *schema, int attrNum, int *offset) {
    attrOffset(schema, attrNum, offset);
    return RC_OK;
}

RC extractAttributeValue(Record *record, Schema *schema, int attrNum, int offset, Value **value) {
    return extractAttrValue(record, schema, attrNum, offset, value);
}

RC getAttr(Record *record, Schema *schema, int attrNum, Value **value) {
    int offset = 0;

    // Validate pointers and attribute number
    RC result = validatePointersAndAttrNum(schema, record, attrNum);
    if (result != RC_OK) {
        return result;
    }

    // Get the attribute's offset
    result = getAttributeOffset(schema, attrNum, &offset);
    if (result != RC_OK) {
        return result;
    }

    // Extract the attribute value and return
    return extractAttributeValue(record, schema, attrNum, offset, value);
}

char schpointer='NA'; 

RC checkSchemaPointer(Schema *schema) {
    // Ensure that schema pointer is not null
    if (schema == NULL) {
        RC_message = "The Schema pointer received is null.";
        return RC_NULL_POINTER;
    }
    return RC_OK;
}

bool valid_attr=true;

RC checkValidAttrNum(int attrNum) {
    // Ensure that attribute number is not less than 0
    if (attrNum < 0) {
        RC_message = "Attribute number cannot be negative.";
        return RC_IM_KEY_NOT_FOUND;
    }
    return RC_OK;
}

void handleIntDataType(Record *record, Value *value, int offset, int *tempVar) {
    *((int *)&record->data[offset]) = value->v.intV;
    *tempVar += value->v.intV;
}

void handleStringDataType(Record *record, Value *value, int offset, int *tempVar) {
    int tempLength = strlen(value->v.stringV);
    for (int i = 0; i < tempLength; i++) {
        record->data[offset + i] = value->v.stringV[i];
    }
    record->data[offset + tempLength] = '\0';
    *tempVar *= 2; // Example operation to differentiate it
}

void handleFloatDataType(Record *record, Value *value, int offset, int *tempVar) {
    *((float *)&record->data[offset]) = value->v.floatV;
    *tempVar += (int)value->v.floatV;
}

void handleBoolDataType(Record *record, Value *value, int offset, int *tempVar) {
    *((bool *)&record->data[offset]) = value->v.boolV;
    *tempVar -= (value->v.boolV ? 1 : 0);
}

RC setAttributeValue(Record *record, Value *value, int offset) {
    int tempVar = 0;

    if (value->dt == DT_INT) {
        handleIntDataType(record, value, offset, &tempVar);
    } else if (value->dt == DT_STRING) {
        handleStringDataType(record, value, offset, &tempVar);
    } else if (value->dt == DT_FLOAT) {
        handleFloatDataType(record, value, offset, &tempVar);
    } else if (value->dt == DT_BOOL) {
        handleBoolDataType(record, value, offset, &tempVar);
    }

    tempVar += 1;
    return RC_OK;
}


char setattr_type='1';

RC validateSchema(Schema *schema) {
    return checkSchemaPointer(schema);
}

void calculateAttrOffset(Schema *schema, int attrNum, int *attributeOffset) {
    attrOffset(schema, attrNum, attributeOffset);
}

RC setAttribute(Record *record, Value *value, int attributeOffset) {
    return setAttributeValue(record, value, attributeOffset);
}

RC setAttr(Record *record, Schema *schema, int attrNum, Value *value) {
    RC result = validateSchema(schema);
    if (result != RC_OK) {
        return result;
    }

    if (record == NULL || value == NULL) {
        RC_message = "Record or Value pointer is NULL.";
        return RC_NULL_POINTER;
    }

    result = validateAttrNum(attrNum);
    if (result != RC_OK) {
        return result;
    }

    int attributeOffset = 0;
    calculateAttrOffset(schema, attrNum, &attributeOffset);

    return setAttribute(record, value, attributeOffset);
}
