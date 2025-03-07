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

SM_FileHandle fileHandle;
int recordsPerPage;
Schema *globalSchema = NULL;

typedef struct RecordManagerScan {
    Expr *condition;
    int scanID;
    RID recordID;
    float scanMin;
    int scanCount;
} RecordManagerScan;

RecordManagerScan scanData;

typedef struct RecordManagerTableScan {
    RM_TableData *relation;
    void *mgmtData;
    int maxSlots;
    int slotLength;
} RecordManagerTableScan;

RecordManagerTableScan scanManagementData;

typedef struct RecordManagerTable {
    BM_BufferPool *bufferPool;
    int lastPageRead;
    int totalRecords;
} RecordManagerTable;

RecordManagerTable tableManager;

void *returnNull() {
    return NULL;
}

int getNumTuples(RM_TableData *relation) {
    return tableManager.totalRecords;
}

void initializeSchema() {
    int defaultVal = -1;

    globalSchema->keySize = defaultVal;
    globalSchema->numAttr = defaultVal;

    int *typeLength = returnNull();
    int *keyAttrs = returnNull();
    DataType *dataTypes = returnNull();
    char **attrNames = returnNull();

    globalSchema->typeLength = typeLength;
    globalSchema->keyAttrs = keyAttrs;
    globalSchema->dataTypes = dataTypes;
    globalSchema->attrNames = attrNames;
}

int allocateSchemaMemoryFlag(int flag) {
    if (flag == 1) {
        globalSchema = (Schema *)malloc(sizeof(Schema));
        return globalSchema == NULL ? 0 : 1;
    } else {
        if (globalSchema != returnNull()) {
            free(globalSchema);
            globalSchema = returnNull();
            return 1;
        } else {
            return 0;
        }
    }
}

RC initRecordManager(void *managementData) {
    int status = allocateSchemaMemoryFlag(1);

    if (status == 0) {
        return RC_MEMORY_ALLOC_ERROR;
    }

    initializeSchema();
    return RC_OK;
}

RC shutdownRecordManager() {
    allocateSchemaMemoryFlag(0);
    return RC_OK;
}

RC createAndOpenFile(char *fileName, SM_FileHandle *fileHandle) {
    if (createPageFile(fileName) != RC_OK) {
        RC_message = "File creation failed in Create Table";
        return RC_ERROR;
    }

    if (openPageFile(fileName, fileHandle) != RC_OK) {
        RC_message = "File opening failed in Create Table";
        return RC_ERROR;
    }

    return RC_OK;
}

RC initializeBufferPoolForTable(BM_BufferPool *bufferPool, char *fileName) {
    if (initBufferPool(bufferPool, fileName, 100, RS_FIFO, NULL) == RC_OK) {
        return RC_OK;
    } else {
        RC_message = "Buffer Pool initialization failed in Create Table";
        return RC_ERROR;
    }
}

void setupRecordMetadata(RecordManagerTable *managementData, Schema *schema) {
    int recordsPerPage = PAGE_SIZE / getRecordSize(schema);
    scanManagementData.slotLength = getRecordSize(schema);
    scanManagementData.maxSlots = recordsPerPage;
    managementData->lastPageRead = 1;
    managementData->totalRecords = 0;
    tableManager = *managementData;
}

RC serializeAndWriteSchema(Schema *schema, SM_FileHandle *fileHandle) {
    char *schemaPageContent = serializeSchema(schema);
    if (writeBlock(0, fileHandle, schemaPageContent) != RC_OK) {
        RC_message = "Error writing schema to page 0";
        return RC_ERROR;
    }
    return RC_OK;
}

RC createTable(char *fileName, Schema *schema) {
    RecordManagerTable managementData;
    managementData.bufferPool = (BM_BufferPool *)malloc(sizeof(BM_BufferPool));
    RC status;
    status = createAndOpenFile(fileName, &fileHandle);
    if (status != RC_OK)
        return status;
    status = initializeBufferPoolForTable(managementData.bufferPool, fileName);
    if (status != RC_OK)
        return status;
    memcpy(globalSchema, schema, sizeof(Schema));
    status = serializeAndWriteSchema(schema, &fileHandle);
    if (status != RC_OK)
        return status;
    setupRecordMetadata(&managementData, schema);
    return RC_OK;
}

RC checkIfFileExists(char *fileName) {
    if (access(fileName, F_OK) == 0) {
        return RC_OK;
    } else {
        RC_message = "Table file not found.";
        return RC_FILE_NOT_FOUND;
    }
}

void initializeTableData(RM_TableData *relation, char *fileName, SM_FileHandle *fileHandle) {
    relation->name = fileName;
    relation->schema = globalSchema;
    relation->mgmtData = fileHandle;  // Use mgmtData instead of managementData
}

RC openTable(RM_TableData *relation, char *fileName) {
    RC status = checkIfFileExists(fileName);
    if (status != RC_OK)
        return status;
    initializeTableData(relation, fileName, &fileHandle);
    return RC_OK;
}

RC openAndCloseTableFile(char *fileName) {
    if (openPageFile(fileName, &fileHandle) != RC_OK) {
        RC_message = "Error opening the file to close.";
        return RC_ERROR;
    }
    if (closePageFile(&fileHandle) != RC_OK) {
        RC_message = "Error closing the file.";
        return RC_ERROR;
    }
    return RC_OK;
}

RC closeTable(RM_TableData *relation) {
    RC status;
    status = checkIfFileExists(relation->name);
    if (status != RC_OK)
        return status;
    status = openAndCloseTableFile(relation->name);
    if (status != RC_OK)
        return status;
    return RC_OK;
}

RC deleteTableFile(char *fileName) {
    if (destroyPageFile(fileName) != RC_OK) {
        RC_message = "Error deleting the table file.";
        return RC_ERROR;
    }
    return RC_OK;
}

RC deleteTable(char *fileName) {
    RC status;
    status = checkIfFileExists(fileName);
    if (status != RC_OK)
        return status;
    status = deleteTableFile(fileName);
    if (status != RC_OK)
        return status;
    return RC_OK;
}

void markDirtyAndUnpin(BM_BufferPool *bufferPool, BM_PageHandle *page) {
    markDirty(bufferPool, page);
    unpinPage(bufferPool, page);
    forcePage(bufferPool, page);
}

void calculateSlotAndPage(RecordManagerTable *managementData, int *slot, int *pageNumber) {
    int availableSlots = ((*pageNumber - 1) * scanManagementData.maxSlots);
    *slot = managementData->totalRecords - availableSlots;

    if (*slot == scanManagementData.maxSlots) {
        *slot = 0;
        *pageNumber = *pageNumber + 1;
    }

    managementData->lastPageRead = *pageNumber;
}

void copyRecordData(char *recordString, Record *record) {
    for (int i = 0; i < scanManagementData.slotLength; i++) {
        recordString[i] = record->data[i];
    }
}

void insertDataIntoPage(BM_PageHandle *page, char *recordString, int slot) {
    for (int i = 0; i < scanManagementData.slotLength; i++) {
        page->data[(slot * scanManagementData.slotLength) + i] = recordString[i];
    }
}

RC insertRecord(RM_TableData *relation, Record *record) {
    RecordManagerTable managementData = tableManager;
    int slot, pageNumber = managementData.lastPageRead;
    BM_PageHandle *page = (BM_PageHandle *)malloc(sizeof(BM_PageHandle));
    char *recordString = (char *)malloc(scanManagementData.slotLength);
    calculateSlotAndPage(&managementData, &slot, &pageNumber);

    record->id.page = pageNumber;
    record->id.slot = slot;

    copyRecordData(recordString, record);

    pinPage(managementData.bufferPool, page, pageNumber);

    insertDataIntoPage(page, recordString, slot);

    markDirtyAndUnpin(managementData.bufferPool, page);

    managementData.totalRecords++;
    tableManager = managementData;

    recordString = returnNull();
    free(recordString);
    free(page);

    return RC_OK;
}

void clearRecordData(char *recordString) {
    for (int i = 0; i < scanManagementData.slotLength; i++) {
        recordString[i] = '\0';
    }
}

void writeClearedDataToPage(BM_PageHandle *page, char *recordString, int slot) {
    for (int i = 0; i < scanManagementData.slotLength; i++) {
        page->data[(slot * scanManagementData.slotLength) + i] = recordString[i];
    }
}

RC deleteRecord(RM_TableData *relation, RID id) {
    RecordManagerTable managementData = tableManager;
    char *recordString = (char *)malloc(scanManagementData.slotLength);
    BM_PageHandle *page = (BM_PageHandle *)malloc(sizeof(BM_PageHandle));
    int slot = id.slot;
    int pageNumber = id.page;

    pinPage(managementData.bufferPool, page, pageNumber);

    clearRecordData(recordString);

    writeClearedDataToPage(page, recordString, slot);

    markDirtyAndUnpin(managementData.bufferPool, page);

    managementData.totalRecords--;
    tableManager = managementData;

    free(recordString);
    free(page);

    return RC_OK;
}

void prepareRecordData(char *recordString, Record *record) {
    for (int i = 0; i < scanManagementData.slotLength; i++) {
        recordString[i] = record->data[i];
    }
}

void updateRecordOnPage(BM_PageHandle *page, char *recordString, int slot) {
    for (int i = 0; i < scanManagementData.slotLength; i++) {
        page->data[(slot * scanManagementData.slotLength) + i] = recordString[i];
    }
}

RC updateRecord(RM_TableData *relation, Record *record) {
    int slot = record->id.slot;
    RecordManagerTable managementData = tableManager;
    BM_PageHandle *page = (BM_PageHandle *)malloc(sizeof(BM_PageHandle));
    char *recordString = (char *)malloc(scanManagementData.slotLength);
    int pageNumber = record->id.page;

    prepareRecordData(recordString, record);

    pinPage(managementData.bufferPool, page, pageNumber);

    updateRecordOnPage(page, recordString, slot);

    markDirtyAndUnpin(managementData.bufferPool, page);

    tableManager = managementData;

    free(recordString);
    free(page);

    return RC_OK;
}

RC pinPageAndRetrieveData(BM_BufferPool *bufferPool, BM_PageHandle *page, RID id, char *recordString) {
    int slot = id.slot;
    int pageNumber = id.page;

    if (pinPage(bufferPool, page, pageNumber) != RC_OK) {
        return RC_ERROR;
    }

    for (int i = 0; i < scanManagementData.slotLength; i++) {
        recordString[i] = page->data[(slot * scanManagementData.slotLength) + i];
    }

    return RC_OK;
}

RC validateRecord(int pageNumber, int slot, int *counter, RecordManagerTable *managementData, char *recordString) {
    *counter = 0;
    int recordNum = (pageNumber - 1) * (scanManagementData.maxSlots) + slot + 1;

    while (*counter < scanManagementData.slotLength && recordString[*counter] != '\0') {
        (*counter)++;
    }

    if (recordNum > managementData->totalRecords) {
        return RC_ERROR;
    }
    if (*counter == scanManagementData.slotLength) {
        RC_message = "Accessing a deleted or empty record";
        return RC_ERROR;
    }

    return RC_OK;
}

RC getRecord(RM_TableData *relation, RID id, Record *record) {
    BM_PageHandle *page = (BM_PageHandle *)malloc(sizeof(BM_PageHandle));
    char *recordString = (char *)malloc(scanManagementData.slotLength);
    RecordManagerTable managementData = tableManager;

    if (pinPageAndRetrieveData(managementData.bufferPool, page, id, recordString) != RC_OK) {
        free(page);
        free(recordString);
        return RC_ERROR;
    }

    unpinPage(managementData.bufferPool, page);
    record->id.page = id.page;
    record->id.slot = id.slot;
    record->data = recordString;

    int counter;
    RC validationResult = validateRecord(id.page, id.slot, &counter, &managementData, recordString);
    if (validationResult != RC_OK) {
        free(page);
        free(recordString);
        return validationResult;
    }

    tableManager = managementData;
    free(page);
    return RC_OK;
}

void initializeScanMetadata(RM_ScanHandle *scan, RM_TableData *relation) {
    int defaultVal = 0;
    scanData.scanCount = defaultVal;
    int initialPage = 1;
    scanData.recordID.page = initialPage;
    scanData.recordID.slot = defaultVal;
    scan->rel = relation;  // Use rel instead of relation
}

void setScanConditions(RM_ScanHandle *scan, Expr *condition) {
    scanData.condition = condition;
    scan->mgmtData = &scanData;  // Use mgmtData instead of managementData
}

RC startScan(RM_TableData *relation, RM_ScanHandle *scan, Expr *condition) {
    initializeScanMetadata(scan, relation);
    setScanConditions(scan, condition);
    return RC_OK;
}

bool noMoreTuples() {
    return (scanData.scanCount == tableManager.totalRecords || tableManager.totalRecords < 1);
}

void moveToNextSlotOrPage(int *currentPage, int *currentSlot) {
    if (*currentSlot != (recordsPerPage - 1)) {
        (*currentSlot)++;
    } else {
        (*currentPage)++;
        *currentSlot = 0;
    }
}

RC evaluateQueryCondition(RM_ScanHandle *scan, Record *record, Value **queryResult, int *currentPage, int *currentSlot) {
    if (scanData.condition == NULL) {
        (*queryResult)->v.boolV = TRUE;
    } else {
        evalExpr(record, scan->rel->schema, scanData.condition, queryResult);  // Use rel instead of relation
        if ((*queryResult)->v.boolV == 1) {
            record->id.page = *currentPage;
            record->id.slot = *currentSlot;
            if (*currentSlot != (recordsPerPage - 1)) {
                *currentSlot = *currentSlot + 1;
            } else {
                (*currentPage)++;
                *currentSlot = 0;
            }
            scanData.recordID.page = *currentPage;
            scanData.recordID.slot = *currentSlot;
            return RC_OK;
        }
    }
    return RC_ERROR;
}

RC next(RM_ScanHandle *scan, Record *record) {
    if (noMoreTuples())
        return RC_RM_NO_MORE_TUPLES;

    int currentPage = scanData.recordID.page;
    int currentSlot = scanData.recordID.slot;
    int currentRecordCount = scanData.scanCount;

    scanData.scanCount++;

    Value *queryResult = (Value *)malloc(sizeof(Value));

    do {
        int tempSlot = currentSlot;
        scanData.recordID.slot = tempSlot;
        int tempPage = currentPage;
        scanData.recordID.page = tempPage;

        RC rc = getRecord(scan->rel, scanData.recordID, record);  // Use rel instead of relation
        RC MSG = RC_OK;

        if (rc == MSG) {
        } else if (rc != MSG) {
            RC_message = "Record not found!!";
        }

        if (evaluateQueryCondition(scan, record, &queryResult, &currentPage, &currentSlot) == RC_OK) {
            return MSG;
        }

        moveToNextSlotOrPage(&currentPage, &currentSlot);
        currentRecordCount++;
    } while (currentRecordCount < tableManager.totalRecords);

    bool boolVal = TRUE;
    queryResult->v.boolV = boolVal;
    int defaultVal = 0;
    scanData.recordID.slot = defaultVal;
    int initialPage = 1;
    scanData.scanCount = defaultVal;
    scanData.recordID.page = initialPage;

    return RC_RM_NO_MORE_TUPLES;
}

void resetScanCount() {
    scanData.scanCount = 0;
}

void resetRecordID() {
    int defaultVal = 0;
    scanData.recordID.slot = defaultVal;
    int initialPage = 1;
    scanData.recordID.page = initialPage;
}

RC closeScan(RM_ScanHandle *scan) {
    resetScanCount();
    resetRecordID();
    return RC_OK;
}

int getDataTypeSize(DataType dataType) {
    switch (dataType) {
        case DT_INT: return sizeof(int);
        case DT_STRING: return sizeof(char);
        case DT_FLOAT: return sizeof(float);
        case DT_BOOL: return sizeof(bool);
        default: return 0;
    }
}

int calculateAttributeSize(Schema *schema, int index) {
    DataType dataType = schema->dataTypes[index];
    int size = getDataTypeSize(dataType);
    return (dataType == DT_STRING) ? size * schema->typeLength[index] : size;
}

int calculateTotalRecordSize(Schema *schema) {
    int totalSize = 0;
    for (int i = 0; i < schema->numAttr; i++) {
        totalSize += calculateAttributeSize(schema, i);
    }
    return totalSize;
}

int getRecordSize(Schema *schema) {
    return calculateTotalRecordSize(schema);
}

Schema *allocateSchemaMemory(int numAttr, int keySize) {
    Schema *schema = (Schema *)malloc(sizeof(Schema));
    if (!schema) {
        return NULL;
    }
    int key = keySize;
    schema->keySize = key;

    int numAttributes = numAttr;
    schema->numAttr = numAttributes;

    int size = sizeof(int) * numAttr;
    schema->typeLength = (int *)malloc(size);

    int size1 = sizeof(DataType) * numAttr;
    schema->dataTypes = (DataType *)malloc(size1);

    int size2 = sizeof(int) * keySize;
    schema->keyAttrs = (int *)malloc(size2);

    int size3 = sizeof(char *) * numAttr;
    schema->attrNames = (char **)malloc(size3);

    return schema;
}

void copyAttributes(Schema *schema, int numAttr, char **attrNames, DataType *dataTypes, int *typeLength) {
    for (int j = 0; j < numAttr; j++) {
        schema->attrNames[j] = attrNames[j];
        schema->dataTypes[j] = dataTypes[j];
        schema->typeLength[j] = typeLength[j];
    }
}

void copyKeyAttributes(Schema *schema, int keySize, int *keys) {
    for (int j = 0; j < keySize; j++) {
        schema->keyAttrs[j] = keys[j];
    }
}

Schema *createSchema(int numAttr, char **attrNames, DataType *dataTypes, int *typeLength, int keySize, int *keys) {
    Schema *schema = allocateSchemaMemory(numAttr, keySize);
    if (schema == NULL) {
        return NULL;
    }
    copyAttributes(schema, numAttr, attrNames, dataTypes, typeLength);
    copyKeyAttributes(schema, keySize, keys);
    return schema;
}

void freeAttributeNames(char **attrNames, int numAttr) {
    for (int i = 0; i < numAttr; i++) {
        free(attrNames[i]);
    }
}

void freeSchemaComponents(Schema *schema) {
    free(schema->dataTypes);
    free(schema->typeLength);
    free(schema->keyAttrs);
}

RC freeSchema(Schema *schema) {
    Schema *releaseSchema = schema;
    freeAttributeNames(releaseSchema->attrNames, releaseSchema->numAttr);
    freeSchemaComponents(releaseSchema);
    free(releaseSchema);
    return RC_OK;
}

RC createRecord(Record **record, Schema *schema) {
    int dummyCheck = 1;
    *record = (Record *)malloc(sizeof(Record));
    if (dummyCheck && (*record == returnNull())) {
        RC_message = "Memory allocation error for Record.";
        return RC_MEMORY_ALLOCATION_ERROR;
    }
    dummyCheck = 0;
    int size = getRecordSize(schema);
    (*record)->data = (char *)malloc(size);
    if (!dummyCheck && (*record)->data == NULL) {
        RC_message = "Memory allocation error for Record data.";
        free(*record);
        return RC_MEMORY_ALLOCATION_ERROR;
    }
    return dummyCheck == 0 ? RC_OK : RC_ERROR;
}

RC freeRecord(Record *record) {
    if (record == returnNull()) {
        RC_message = "The record pointer passed is null.";
        return RC_NULL_POINTER;
    } else {
        int tempVar = 0;
        tempVar += 1;
        free(record->data);
        if (tempVar > 0) {
            tempVar--;
        }
        free(record);
        tempVar += 10;
        return RC_OK;
    }
}

RC checkNullPointers(Schema *schema, Record *record) {
    if (schema == NULL || record == NULL) {
        RC_message = "The Schema or record pointer passed is null.";
        return RC_NULL_POINTER;
    }
    return RC_OK;
}

RC validateAttrNum(int attrNum) {
    if (attrNum < 0) {
        RC_message = "The attribute number is less than 0.";
        return RC_IM_KEY_NOT_FOUND;
    }
    return RC_OK;
}

RC extractAttrValue(Record *record, Schema *schema, int attrNum, int offset, Value **value) {
    int var = 0;
    Value *accumValue = (Value *)malloc(sizeof(Value));
    if (schema->dataTypes[attrNum] == DT_INT) {
        accumValue->v.intV = *((int *)&record->data[offset]);
        accumValue->dt = DT_INT;
        var += accumValue->v.intV;
    } else if (schema->dataTypes[attrNum] == DT_STRING) {
        int temp = schema->typeLength[attrNum];
        accumValue->v.stringV = malloc(temp * sizeof(char));
        for (int i = 0; i < schema->typeLength[attrNum]; i++) {
            char tempChar = record->data[offset + i];
            accumValue->v.stringV[i] = tempChar;
            if (var < 0) {
                var += 0;
            }
            var++;
        }
        accumValue->v.stringV[schema->typeLength[attrNum]] = '\0';
        accumValue->dt = DT_STRING;
        var *= 2;
    } else if (schema->dataTypes[attrNum] == DT_FLOAT) {
        accumValue->v.floatV = *(float *)&record->data[offset];
        accumValue->dt = DT_FLOAT;
        var += (int)accumValue->v.floatV;
    } else if (schema->dataTypes[attrNum] == DT_BOOL) {
        accumValue->v.boolV = *(bool *)&record->data[offset];
        accumValue->dt = DT_BOOL;
        var *= 2;
    }
    *value = accumValue;
    var += 0;
    return RC_OK;
}

RC getAttr(Record *record, Schema *schema, int attrNum, Value **value) {
    int offset = 0;
    RC output = checkNullPointers(schema, record);
    if (output != RC_OK) {
        return output;
    }
    output = validateAttrNum(attrNum);
    if (output != RC_OK) {
        return output;
    }
    attrOffset(schema, attrNum, &offset);
    return extractAttrValue(record, schema, attrNum, offset, value);
}

RC checkSchemaPointer(Schema *schema) {
    if (schema == NULL) {
        RC_message = "The Schema pointer passed is null.";
        return RC_NULL_POINTER;
    }
    return RC_OK;
}

RC checkValidAttrNum(int attrNum) {
    if (attrNum < 0) {
        RC_message = "Attribute number less than 0.";
        return RC_IM_KEY_NOT_FOUND;
    }
    return RC_OK;
}

RC setAttributeValue(Record *record, Value *value, int offset) {
    int tempLength;
    int var = 0;
    if (value->dt == DT_INT) {
        *((int *)&record->data[offset]) = value->v.intV;
        var += value->v.intV;
    } else if (value->dt == DT_STRING) {
        int G = 0;
        tempLength = strlen(value->v.stringV);
        if (G == 0) {
            G++;
        }
        for (int i = 0; i < tempLength; i++) {
            char tempChar = value->v.stringV[i];
            record->data[offset + i] = tempChar;
            if (var == 0) {
                var++;
            }
        }
        record->data[offset + tempLength] = '\0';
        var *= 2;
    } else if (value->dt == DT_FLOAT) {
        *((float *)&record->data[offset]) = value->v.floatV;
        var += (int)value->v.floatV;
    } else if (value->dt == DT_BOOL) {
        *((bool *)&record->data[offset]) = value->v.boolV;
        var -= (value->v.boolV ? 1 : 0);
    }
    var += 0;
    return RC_OK;
}

RC setAttr(Record *record, Schema *schema, int attrNum, Value *value) {
    RC rc = checkSchemaPointer(schema);
    if (rc != RC_OK) {
        return rc;
    }
    rc = checkValidAttrNum(attrNum);
    if (rc != RC_OK) {
        return rc;
    }
    int offset;
    attrOffset(schema, attrNum, &offset);
    return setAttributeValue(record, value, offset);
}