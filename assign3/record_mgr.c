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


SM_FileHandle fHandle;
int no_of_records_per_page;
Schema *schma_g17 = NULL;

typedef struct Recrd_mgr_scan_g17
{
    Expr *condition_g17;
    int scan_i_d_g17;
    RID recrd_i_d_g17;
    float scan_m_i_n_g17;
    int scan_count_g17;

} Recrd_mgr_scan_g17;

Recrd_mgr_scan_g17 scan_MG_Data_g17;

typedef struct record_mgr_table_scan
{
    RM_TableData *rel;
    void *mgmtData;
    int max_no_of_slots_g17;
    int length_of_slot_g17;
} record_mgr_table_scan;
record_mgr_table_scan scan_Mgmt_Data;

typedef struct recrd_mgr_tablemgr_g17
{
    BM_BufferPool *bm;
    int final_page_recrd_read_g17;
    int no_of_recrds_g17;

} recrd_mgr_tablemgr_g17;
recrd_mgr_tablemgr_g17 recrd_table_mgr_g17;

void *return_null_g17()
{
    return NULL;
}

int getNumTuples(RM_TableData *rel)
{
    return recrd_table_mgr_g17.no_of_recrds_g17;
}

void initalize_schema()
{
    int a_g17 = -1;

    schma_g17->keySize = a_g17;
    schma_g17->numAttr = a_g17;

    // Assuming these are of the correct types
    int *typeLength = return_null_g17();
    int *keyAttrs = return_null_g17();
    DataType *dataTypes = return_null_g17();
    char **attrNames = return_null_g17();

    // Assign values to schma_g17 members
    schma_g17->typeLength = typeLength;

    schma_g17->keyAttrs = keyAttrs;

    schma_g17->dataTypes = dataTypes;
    
    schma_g17->attrNames = attrNames;


}

int schema_memory_alloc(int a)
{
    if (a == 1)
    {
        schma_g17 = (Schema *)malloc(sizeof(Schema));
        return schma_g17 == NULL ? 0 : 1;
    }
    else
    {
        if (schma_g17 != return_null_g17())
        {
            free(schma_g17);
            schma_g17 = return_null_g17();
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

RC initRecordManager(void *mgmtData)
{
    int a = schema_memory_alloc(1);

    if (a == 0)
    {
        return RC_MEMORY_ALLOC_ERROR;
    }

    initalize_schema();
    return RC_OK;
}

RC shutdownRecordManager()
{
    schema_memory_alloc(0);

    return RC_OK;
}

RC createAndOpenFile(char *name, SM_FileHandle *fHandle)
{
    if (createPageFile(name) != RC_OK)
    {
        RC_message = "File could not be created in Create Table";
        return RC_ERROR;
    }

    if (openPageFile(name, fHandle) != RC_OK)
    {
        RC_message = "File could not be opened in Create Table";
        return RC_ERROR;
    }

    return RC_OK;
}

RC initializeBufferPoolForTable(BM_BufferPool *bm, char *name)
{
    if (initBufferPool(bm, name, 100, RS_FIFO, NULL) == RC_OK)
    {
        return RC_OK;
    }
    else
    {
        RC_message = "Buffer Pool cannot be initialized in Create Table Function";
        return RC_ERROR;
    }
}
void setupRecordMetadata(recrd_mgr_tablemgr_g17 *mgmtdata, Schema *schema)
{
    int no_of_records_per_page = PAGE_SIZE / getRecordSize(schema);
    scan_Mgmt_Data.length_of_slot_g17 = getRecordSize(schema);
    scan_Mgmt_Data.max_no_of_slots_g17 = no_of_records_per_page;
    mgmtdata->final_page_recrd_read_g17 = 1;
    mgmtdata->no_of_recrds_g17 = 0;
    recrd_table_mgr_g17 = *mgmtdata;
}

RC serializeAndWriteSchema(Schema *schema, SM_FileHandle *fHandle)
{
    char *schemaPageContent = serializeSchema(schema);
    if (writeBlock(0, fHandle, schemaPageContent) != RC_OK)
    {
        RC_message = "Error writing schema to page 0";
        return RC_ERROR;
    }
    return RC_OK;
}

RC createTable(char *name, Schema *schema)
{
    recrd_mgr_tablemgr_g17 mgmtdata;
    mgmtdata.bm = (BM_BufferPool *)malloc(sizeof(BM_BufferPool));
    RC status;
    status = createAndOpenFile(name, &fHandle);
    if (status != RC_OK)
        return status;
    status = initializeBufferPoolForTable(mgmtdata.bm, name);
    if (status != RC_OK)
        return status;
    memcpy(schma_g17, schema, sizeof(Schema));
    status = serializeAndWriteSchema(schema, &fHandle);
    if (status != RC_OK)
        return status;
    setupRecordMetadata(&mgmtdata, schema);
    return RC_OK;
}

RC checkIfFileExists(char *name)
{
    if (access(name, F_OK) == 0)
    {
        return RC_OK;
    }
    else
    {
        RC_message = "Table we are looking for does not have a file.";
        return RC_FILE_NOT_FOUND;
    }
}

void initializeTableData(RM_TableData *rel, char *name, SM_FileHandle *fHandle)
{
    rel->name = name;
    rel->schema = schma_g17;
    rel->mgmtData = fHandle;
}

RC openTable(RM_TableData *rel, char *name)
{
    RC status = checkIfFileExists(name);
    if (status != RC_OK)
        return status;
    initializeTableData(rel, name, &fHandle);
    return RC_OK;
}

RC openAndCloseTableFile(char *name)
{
    if (openPageFile(name, &fHandle) != RC_OK)
    {
        RC_message = "Error opening the file to close.";
        return RC_ERROR;
    }
    if (closePageFile(&fHandle) != RC_OK)
    {
        RC_message = "Error closing the file.";
        return RC_ERROR;
    }
    return RC_OK;
}

RC closeTable(RM_TableData *rel)
{
    RC status;
    status = checkIfFileExists(rel->name);
    if (status != RC_OK)
        return status;
    status = openAndCloseTableFile(rel->name);
    if (status != RC_OK)
        return status;
    return RC_OK;
}

RC deleteTableFile(char *name)
{
    if (destroyPageFile(name) != RC_OK)
    {
        RC_message = "Error deleting the table file.";
        return RC_ERROR;
    }
    return RC_OK;
}

RC deleteTable(char *name)
{
    RC status;
    status = checkIfFileExists(name);
    if (status != RC_OK)
        return status;
    status = deleteTableFile(name);
    if (status != RC_OK)
        return status;
    return RC_OK;
}

void dirty_unpin_force_g17(BM_BufferPool *bm, BM_PageHandle *page)
{
    markDirty(bm, page);
    unpinPage(bm, page);
    forcePage(bm, page);
}
void calculateSlotAndPage(recrd_mgr_tablemgr_g17 *mgr_data_g17, int *slot_g17, int *pg_number_g17)
{
    int slot_available_g17 = ((*pg_number_g17 - 1) * scan_Mgmt_Data.max_no_of_slots_g17);
    *slot_g17 = mgr_data_g17->no_of_recrds_g17 - slot_available_g17;

    if (*slot_g17 == scan_Mgmt_Data.max_no_of_slots_g17)
    {
        *slot_g17 = 0;
        *pg_number_g17 = *pg_number_g17 + 1;
    }

    mgr_data_g17->final_page_recrd_read_g17 = *pg_number_g17;
}
void copyRecordToString(char *string_record_g17, Record *record)
{
    // Copy the record data to the string variable
    for (int i = 0; i < scan_Mgmt_Data.length_of_slot_g17; i++)
    {
        string_record_g17[i] = record->data[i];
    }
}
void insertDataIntoPage(BM_PageHandle *page, char *string_record_g17, int slot_g17)
{
    for (int i = 0; i < scan_Mgmt_Data.length_of_slot_g17; i++)
    {
        page->data[(slot_g17 * scan_Mgmt_Data.length_of_slot_g17) + i] = string_record_g17[i];
    }
}
RC insertRecord(RM_TableData *rel, Record *record)
{
    recrd_mgr_tablemgr_g17 mgr_data_g17 = recrd_table_mgr_g17;
    
    int slot_g17, pg_number_g17 = mgr_data_g17.final_page_recrd_read_g17;
    
    BM_PageHandle *page = (BM_PageHandle *)malloc(sizeof(BM_PageHandle));
    
    char *string_record_g17 = (char *)malloc(scan_Mgmt_Data.length_of_slot_g17);
    calculateSlotAndPage(&mgr_data_g17, &slot_g17, &pg_number_g17);

    record->id.page = pg_number_g17;
    record->id.slot = slot_g17;

    copyRecordToString(string_record_g17, record);

    pinPage(mgr_data_g17.bm, page, pg_number_g17);

    insertDataIntoPage(page, string_record_g17, slot_g17);

    dirty_unpin_force_g17(mgr_data_g17.bm, page);

    mgr_data_g17.no_of_recrds_g17++;
    recrd_table_mgr_g17 = mgr_data_g17;

    string_record_g17 = return_null_g17();
    free(string_record_g17);
    free(page);

    return RC_OK;
}

void clearRecordData(char *string_record_g17)
{
    for (int i = 0; i < scan_Mgmt_Data.length_of_slot_g17; i++)
    {
        string_record_g17[i] = '\0';
    }
}
void writeClearedDataToPage(BM_PageHandle *page, char *string_record_g17, int slot_g17)
{
    for (int i = 0; i < scan_Mgmt_Data.length_of_slot_g17; i++)
    {
        page->data[(slot_g17 * scan_Mgmt_Data.length_of_slot_g17) + i] = string_record_g17[i];
    }
}
RC deleteRecord(RM_TableData *rel, RID id)
{
    recrd_mgr_tablemgr_g17 mgr_data_g17 = recrd_table_mgr_g17;

    char *string_record_g17 = (char *)malloc(scan_Mgmt_Data.length_of_slot_g17);
    
    BM_PageHandle *page = (BM_PageHandle *)malloc(sizeof(BM_PageHandle));

   
    int slot_g17 = id.slot;
    int pg_number_g17 = id.page;

    pinPage(mgr_data_g17.bm, page, pg_number_g17);

    clearRecordData(string_record_g17);

    writeClearedDataToPage(page, string_record_g17, slot_g17);

    dirty_unpin_force_g17(mgr_data_g17.bm, page);

    mgr_data_g17.no_of_recrds_g17--;
    recrd_table_mgr_g17 = mgr_data_g17;

    free(string_record_g17);
    free(page);

    return RC_OK;
}


void prepareRecordData(char *string_record_g17, Record *record)
{
    for (int i = 0; i < scan_Mgmt_Data.length_of_slot_g17; i++)
    {
        string_record_g17[i] = record->data[i];
    }
}
void updateRecordOnPage(BM_PageHandle *page, char *string_record_g17, int slot_g17)
{
    for (int i = 0; i < scan_Mgmt_Data.length_of_slot_g17; i++)
    {
        page->data[(slot_g17 * scan_Mgmt_Data.length_of_slot_g17) + i] = string_record_g17[i];
    }
}
RC updateRecord(RM_TableData *rel, Record *record)
{
    int slot_g17 = record->id.slot;

    recrd_mgr_tablemgr_g17 mgr_data_g17 = recrd_table_mgr_g17;

    BM_PageHandle *page = (BM_PageHandle *)malloc(sizeof(BM_PageHandle));

    char *string_record_g17 = (char *)malloc(scan_Mgmt_Data.length_of_slot_g17);
    
    int pg_number_g17 = record->id.page;

    prepareRecordData(string_record_g17, record);

    pinPage(mgr_data_g17.bm, page, pg_number_g17);

    updateRecordOnPage(page, string_record_g17, slot_g17);

    dirty_unpin_force_g17(mgr_data_g17.bm, page);

   recrd_table_mgr_g17 = mgr_data_g17;

    free(string_record_g17);
    free(page);

    return RC_OK;
}

RC pinPageAndRetrieveData(BM_BufferPool *bm, BM_PageHandle *page, RID id, char *string_record_g17)
{
    int slot_g17 = id.slot;
    int pg_number_g17 = id.page;

    if (pinPage(bm, page, pg_number_g17) != RC_OK) {
        return RC_ERROR;
    }

    for (int i = 0; i < scan_Mgmt_Data.length_of_slot_g17; i++)
    {
        string_record_g17[i] = page->data[(slot_g17 * scan_Mgmt_Data.length_of_slot_g17) + i];
    }

    return RC_OK;
}

RC validateRecord(int pg_number_g17, int slot_g17, int *counter, recrd_mgr_tablemgr_g17 *mgr_data_g17, char *string_record_g17)
{
    *counter = 0;
    int recordNum = (pg_number_g17 - 1) * (scan_Mgmt_Data.max_no_of_slots_g17) + slot_g17 + 1;

    while (*counter < scan_Mgmt_Data.length_of_slot_g17 && string_record_g17[*counter] != '\0')
    {
        (*counter)++;
    }

    if (recordNum > mgr_data_g17->no_of_recrds_g17)
    {
        return RC_ERROR;
    }
    if (*counter == scan_Mgmt_Data.length_of_slot_g17)
    {
        RC_message = "Accessing a deleted or empty record";
        return RC_ERROR;
    }

    return RC_OK;
}

RC getRecord(RM_TableData *rel, RID id, Record *record)
{    
    BM_PageHandle *page = (BM_PageHandle *)malloc(sizeof(BM_PageHandle));
    char *string_record_g17 = (char *)malloc(scan_Mgmt_Data.length_of_slot_g17);
    recrd_mgr_tablemgr_g17 mgr_data_g17 = recrd_table_mgr_g17;

    if (pinPageAndRetrieveData(mgr_data_g17.bm, page, id, string_record_g17) != RC_OK)
    {
        free(page);
        free(string_record_g17);
        return RC_ERROR;
    }

    unpinPage(mgr_data_g17.bm, page);
    record->id.page = id.page;
    record->id.slot = id.slot;
    record->data = string_record_g17;

    int counter;
    RC validationResult = validateRecord(id.page, id.slot, &counter, &mgr_data_g17, string_record_g17);
    if (validationResult != RC_OK)
    {
        free(page);
        free(string_record_g17);
        return validationResult;
    }

    recrd_table_mgr_g17 = mgr_data_g17;
    free(page);
    return RC_OK;
}

void initializeScanMetadata(RM_ScanHandle *scan, RM_TableData *rel)
{
    int a_g17 = 0;
    scan_MG_Data_g17.scan_count_g17 = a_g17;
    int b_g17 = 1;
    scan_MG_Data_g17.recrd_i_d_g17.page = b_g17;
    scan_MG_Data_g17.recrd_i_d_g17.slot = a_g17;
    scan->rel = rel;
}

void setScanConditions(RM_ScanHandle *scan, Expr *condition_g17)
{
    scan_MG_Data_g17.condition_g17 = condition_g17;
    scan->mgmtData = &scan_MG_Data_g17;
}

RC startScan(RM_TableData *rel, RM_ScanHandle *scan, Expr *condition_g17)
{
    initializeScanMetadata(scan, rel);

    setScanConditions(scan, condition_g17);

    return RC_OK;
}

bool noMoreTuples()
{
    return (scan_MG_Data_g17.scan_count_g17 == recrd_table_mgr_g17.no_of_recrds_g17 || recrd_table_mgr_g17.no_of_recrds_g17 < 1);
}

void moveToNextSlotOrPage(int *cur_scan_page_g17, int *cur_scan_slot_g17)
{
    if (*cur_scan_slot_g17 != (no_of_records_per_page - 1))
    {
        (*cur_scan_slot_g17)++;
    }
    else
    {
        (*cur_scan_page_g17)++;
        *cur_scan_slot_g17 = 0;
    }
}
RC evaluateQueryCondition(RM_ScanHandle *scan, Record *record, Value **result_query_g17, int *cur_scan_page_g17, int *cur_scan_slot_g17)
{
    if (scan_MG_Data_g17.condition_g17 == NULL)
    {
        (*result_query_g17)->v.boolV = TRUE;
    }
    else
    {
        evalExpr(record, scan->rel->schema, scan_MG_Data_g17.condition_g17, result_query_g17);
        if ((*result_query_g17)->v.boolV == 1)
        {
            record->id.page = *cur_scan_page_g17;
            record->id.slot = *cur_scan_slot_g17;
            if (*cur_scan_slot_g17 != (no_of_records_per_page - 1)) {
                *cur_scan_slot_g17 = *cur_scan_slot_g17 + 1;
            } 
            else {
                (*cur_scan_page_g17)++;
                *cur_scan_slot_g17 = 0;
            }
            scan_MG_Data_g17.recrd_i_d_g17.page = *cur_scan_page_g17;
            scan_MG_Data_g17.recrd_i_d_g17.slot = *cur_scan_slot_g17;
            return RC_OK;
        }
    }
    return RC_ERROR;
}

RC next(RM_ScanHandle *scan, Record *record)
{
    if (noMoreTuples())
        return RC_RM_NO_MORE_TUPLES;

    int cur_scan_page_g17 = scan_MG_Data_g17.recrd_i_d_g17.page;
    int cur_scan_slot_g17 = scan_MG_Data_g17.recrd_i_d_g17.slot;
    int cur_recrd_sum_scan_g17 = scan_MG_Data_g17.scan_count_g17;

    scan_MG_Data_g17.scan_count_g17++;

    Value *result_query_g17 = (Value *)malloc(sizeof(Value));

    do {
        int temp_slot_g17 = cur_scan_slot_g17;
        scan_MG_Data_g17.recrd_i_d_g17.slot = temp_slot_g17;
        int temp_page_g17 = cur_scan_page_g17;
        scan_MG_Data_g17.recrd_i_d_g17.page = temp_page_g17;
        
        RC rc = getRecord(scan->rel, scan_MG_Data_g17.recrd_i_d_g17, record);
        RC MSG = RC_OK;

        if (rc == MSG) {
        } else if (rc != MSG) {
            RC_message = "Record not found!!";
        }

        if (evaluateQueryCondition(scan, record, &result_query_g17, &cur_scan_page_g17, &cur_scan_slot_g17) == RC_OK) {
            return MSG;
        }

        moveToNextSlotOrPage(&cur_scan_page_g17, &cur_scan_slot_g17);
        cur_recrd_sum_scan_g17++;
    } while (cur_recrd_sum_scan_g17 < recrd_table_mgr_g17.no_of_recrds_g17);

    bool bool_val = TRUE;
    result_query_g17->v.boolV = bool_val;
    int a_g17 = 0;
    scan_MG_Data_g17.recrd_i_d_g17.slot = a_g17;
    int b_g17 = 1;
    scan_MG_Data_g17.scan_count_g17 = a_g17;
    scan_MG_Data_g17.recrd_i_d_g17.page = b_g17;

    return RC_RM_NO_MORE_TUPLES;
}

void resetScanCount()
{
    scan_MG_Data_g17.scan_count_g17 = 0;
}

void resetRecordID()
{
    int a_g17 = 0;
    scan_MG_Data_g17.recrd_i_d_g17.slot = a_g17;
    int b_g17 = 1;
    scan_MG_Data_g17.recrd_i_d_g17.page = b_g17;
}

RC closeScan(RM_ScanHandle *scan)
{
    resetScanCount();
    resetRecordID();
    return RC_OK;
}
int getDataTypeSize(DataType dataType)
{
    switch (dataType)
    {
        case DT_INT: return sizeof(int);
        case DT_STRING: return sizeof(char);
        case DT_FLOAT: return sizeof(float);
        case DT_BOOL: return sizeof(bool);
        default: return 0;
    }
}

int calculateAttributeSize(Schema *schema, int index_g17)
{
    DataType dataType = schema->dataTypes[index_g17];
    int size = getDataTypeSize(dataType);
    return (dataType == DT_STRING) ? size * schema->typeLength[index_g17] : size;
}

int calculateTotalRecordSize(Schema *schema)
{
    int size_total_g17 = 0;
    for (int i = 0; i < schema->numAttr; i++)
    {
        size_total_g17 += calculateAttributeSize(schema, i);
    }
    return size_total_g17;
}

int getRecordSize(Schema *schema)
{
    return calculateTotalRecordSize(schema);
}

Schema *allocateSchemaMemory(int numAttr, int keySize)
{
    Schema *schema = (Schema *)malloc(sizeof(Schema));
    if (!schema)
    {
        return NULL;
    }
    int key_g17 = keySize;
    schema->keySize = key_g17;

    int num_attribute_g17 = numAttr;
    schema->numAttr = num_attribute_g17;

    int size_g17 = sizeof(int) * numAttr;
    schema->typeLength = (int *)malloc(size_g17);

    int size_g17_1 = sizeof(DataType) * numAttr;
    schema->dataTypes = (DataType *)malloc(size_g17_1);

    int size_g17_2 = sizeof(int) * keySize;
    schema->keyAttrs = (int *)malloc(size_g17_2);

    int size_g17_3 = sizeof(char *) * numAttr;
    schema->attrNames = (char **)malloc(size_g17_3);

    return schema;
}

void copyAttributes(Schema *schema, int numAttr, char **attrNames, DataType *dataTypes, int *typeLength)
{
    for (int j = 0; j < numAttr; j++)
    {   
        schema->attrNames[j] = attrNames[j];
        schema->dataTypes[j] = dataTypes[j];
        schema->typeLength[j] = typeLength[j];
    }
}

void copyKeyAttributes(Schema *schema, int keySize, int *keys)
{
    for (int j = 0; j < keySize; j++)
    {
        schema->keyAttrs[j] = keys[j];
    }
}

Schema *createSchema(int numAttr, char **attrNames, DataType *dataTypes, int *typeLength, int keySize, int *keys)
{
    
    Schema *schema = allocateSchemaMemory(numAttr, keySize);
    if (schema == NULL)
    {
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
    Schema *release_schma_g17 = schema;
    freeAttributeNames(release_schma_g17->attrNames, release_schma_g17->numAttr);
    freeSchemaComponents(release_schma_g17);
    free(release_schma_g17);
    return RC_OK;
}

RC createRecord(Record **record, Schema *schema) {
    int dummy_check = 1;
    *record = (Record *)malloc(sizeof(Record));
    if (dummy_check && (*record == return_null_g17())) {
        RC_message = "Memory allocation error for Record.";
        return RC_MEMORY_ALLOCATION_ERROR;
    }
    dummy_check = 0;
    int size_g17 = getRecordSize(schema);
    (*record)->data = (char *)malloc(size_g17);
    if (!dummy_check && (*record)->data == NULL) {
        RC_message = "Memory allocation error for Record data.";
        free(*record);
        return RC_MEMORY_ALLOCATION_ERROR;
    }
    return dummy_check == 0 ? RC_OK : RC_ERROR;
}

RC freeRecord(Record *record) {   
    if (record == return_null_g17()) {
        RC_message = "The record pointer passed is null.";
        return RC_NULL_POINTER;
    } else {
        int temp_var = 0;
        temp_var += 1;
        free(record->data);
        if (temp_var > 0) {
            temp_var--;
        }
        free(record);
        temp_var += 10;
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
    Value *accum_value_g17 = (Value *)malloc(sizeof(Value));
    if (schema->dataTypes[attrNum] == DT_INT) {
        accum_value_g17->v.intV = *((int *)&record->data[offset]);
        accum_value_g17->dt = DT_INT;
        var += accum_value_g17->v.intV;
    } else if (schema->dataTypes[attrNum] == DT_STRING) {
        int tempo = schema->typeLength[attrNum];
        accum_value_g17->v.stringV = malloc(tempo * sizeof(char));
        for (int i = 0; i < schema->typeLength[attrNum]; i++) {
            char temp = record->data[offset + i];
            accum_value_g17->v.stringV[i] = temp;
            if (var < 0) {
                var += 0;
            }
            var++;
        }
        accum_value_g17->v.stringV[schema->typeLength[attrNum]] = '\0';
        accum_value_g17->dt = DT_STRING;
        var *= 2;
    } else if (schema->dataTypes[attrNum] == DT_FLOAT) {
        accum_value_g17->v.floatV = *(float *)&record->data[offset];
        accum_value_g17->dt = DT_FLOAT;
        var += (int)accum_value_g17->v.floatV;
    } else if (schema->dataTypes[attrNum] == DT_BOOL) {
        accum_value_g17->v.boolV = *(bool *)&record->data[offset];
        accum_value_g17->dt = DT_BOOL;
        var *= 2;
    }
    *value = accum_value_g17;
    var += 0;
    return RC_OK;
}

RC getAttr(Record *record, Schema *schema, int attrNum, Value **value) {
    int Offset_Attr_g17 = 0;
    RC output = checkNullPointers(schema, record);
    if (output != RC_OK) {
        return output;
    }
    output = validateAttrNum(attrNum);
    if (output != RC_OK) {
        return output;
    }
    attrOffset(schema, attrNum, &Offset_Attr_g17);
    return extractAttrValue(record, schema, attrNum, Offset_Attr_g17, value);
}

RC checkSchemaPointer(Schema *schema) {
    if (schema == NULL) {
        RC_message = "The Schema pointer passed is null.";
        return RC_NULL_POINTER;
    }
    return RC_OK;
}

RC checkValidAttrNum_g17(int attrNum) {
    if (attrNum < 0) {
        RC_message = "Attribute number less than 0.";
        return RC_IM_KEY_NOT_FOUND;
    }
    return RC_OK;
}

RC setAttributeValue(Record *record, Value *value, int offset) {
    int tempo_length_g17;
    int var = 0;
    if (value->dt == DT_INT) {
        *((int *)&record->data[offset]) = value->v.intV;
        var += value->v.intV;
    } else if (value->dt == DT_STRING) {
        int G_17 = 0;
        tempo_length_g17 = strlen(value->v.stringV);
        if (G_17 == 0) {
            G_17++;
        }
        for (int i_g17 = 0; i_g17 < tempo_length_g17; i_g17++) {
            char temp = value->v.stringV[i_g17];
            record->data[offset + i_g17] = temp;
            if (var == 0) {
                var++;
            }
        }
        record->data[offset + tempo_length_g17] = '\0';
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
    rc = checkValidAttrNum_g17(attrNum);
    if (rc != RC_OK) {
        return rc;
    }
    int Offset_Attr_g17;
    attrOffset(schema, attrNum, &Offset_Attr_g17);
    return setAttributeValue(record, value, Offset_Attr_g17);
}
