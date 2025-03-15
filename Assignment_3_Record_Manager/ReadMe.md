# CS525-G17
# CS 525: Advance Database Organization

## PROGRAMMING ASSIGNMENT 3: RECORD MANAGER 

### Group_17 Team Members:
1. Kori Kamoljonov - A20594489 -33% Contributed
2. Chaitanya Datta Maddukuri - A20568393 - 33% Contributed
3. Vamshi Krishna Cheeti - A20582646 - 33% Contributed


---

## **1. README**


This README provides a comprehensive overview of the Record Manager implementation, highlighting its functionality and the steps required to execute the code. The Record Manager is structured into five key sections: Table Management, Record Handling, Scanning Mechanisms, Schema Management, and Attribute Operations.

---

## Additional Features: 

We have included an Interactive Interface that enables users to define tables, insert, update, and delete records, as well as perform scans. This feature is implemented as a menu-driven command-line interface (CLI) for ease of use.


---


## The Procedure to Run the Code:

Step 1: Switch to the "Assignment3" branch in the GitHub repository and download the necessary files.

Step 2: Run "make clean" to remove any previously compiled files.

Step 3: Compile the program using the make command.

Step 4: Execute the test case 3_1 by running "./test_assign3_1".

Step 5: Run the expression test cases using ./test_expr.

Step 6: Launch the interactive interface using ./Interface.

Step 7: After execution, clean up generated files using make clean and repeat steps 3 to 5 if necessary.

---
## Features To Do

1. "Table and Manager:" Implement functions and methods for managing tables, including creating, deleting, and altering tables.
2. "Record Management:" Handle records in a table, including inserting, deleting, and updating records.
3. "Scans:" Perform scans on tables, including sequential scans and index scans.
4. "Schema Management:" Manage the schema of a table, including creating, altering, and dropping schemas.
5. "Attribute Values:" Handle attribute values in records, including retrieving, modifying, and comparing attribute values.

---
**ADVANCED DATABASE ORGANIZATION**\
**PROGRAMMING ASSIGNMENT III: RECORD MANAGER**


---

## **2. DATA STRUCTURES**

### **2.1 RecordManagerScan**

A structure for managing scan operations on a table.

```c
typedef struct RecordManagerScan {
    Expr *condition;   // Search condition for scanning records
    int scanID;        // Unique identifier for scan instance
    RID recordID;      // Current Record ID in scan
    float scanMin;     // Placeholder for scan-based calculations
    int scanCount;     // Count of scanned records
} RecordManagerScan;
```

### **2.2 RecordManagerTableScan**

A structure for handling metadata about table scans.

```c
typedef struct RecordManagerTableScan {
    RM_TableData *relation; // Table being scanned
    void *mgmtData;         // Internal management data
    int maxSlots;           // Maximum number of records per page
    int slotLength;         // Length of each record slot
} RecordManagerTableScan;
```

### **2.3 RecordManagerTable**

A structure to manage table metadata.

```c
typedef struct RecordManagerTable {
    BM_BufferPool *bufferPool; // Buffer pool for table pages
    int lastPageRead;          // Last accessed page number
    int totalRecords;          // Total records in the table
} RecordManagerTable;
```

---

## **3. RECORD MANAGER FUNCTIONS**

### **3.1 Table and Record Manager Management**

#### `RC initRecordManager(void *mgmtData)`

- **Description**: Initializes the Record Manager by allocating memory and setting up internal structures.
- **Purpose**: Prepares the Record Manager for future table-related operations.
- **Returns**: `RC_OK` on success, error code otherwise.

#### `RC shutdownRecordManager()`

- **Description**: Gracefully shuts down the Record Manager, deallocating all resources.
- **Purpose**: Ensures that no memory leaks occur.
- **Returns**: `RC_OK` on success, error code otherwise.

### **3.2 Table Management**

#### `RC createTable(char *name, Schema *schema)`

- **Description**: Creates a new table, initializing its schema and metadata.
- **Purpose**: Sets up a new table in storage.
- **Returns**: `RC_OK` on success, `RC_ERROR` on failure.

#### `RC openTable(RM_TableData *rel, char *name)`

- **Description**: Opens an existing table for operations.
- **Purpose**: Loads table metadata and buffer management.
- **Returns**: `RC_OK` if successful, `RC_FILE_NOT_FOUND` if the file doesn’t exist.

#### `RC closeTable(RM_TableData *rel)`

- **Description**: Closes an opened table and frees resources.
- **Purpose**: Ensures proper resource cleanup.
- **Returns**: `RC_OK` on success.

#### `RC deleteTable(char *name)`

- **Description**: Deletes a table from storage by removing its page file.
- **Purpose**: Permanently removes a table.
- **Returns**: `RC_OK` if successful, `RC_FILE_NOT_FOUND` otherwise.

### **3.3 Record Operations**

#### `RC insertRecord(RM_TableData *rel, Record *record)`

- **Description**: Inserts a new record into the table.
- **Purpose**: Adds a new record to storage.
- **Returns**: `RC_OK` if successful.

#### `RC deleteRecord(RM_TableData *rel, RID id)`

- **Description**: Deletes a record based on its Record ID.
- **Purpose**: Removes a specific record from storage.
- **Returns**: `RC_OK` if successful.

#### `RC updateRecord(RM_TableData *rel, Record *record)`

- **Description**: Updates an existing record with new data.
- **Purpose**: Modifies a record’s data.
- **Returns**: `RC_OK` if successful.

#### `RC getRecord(RM_TableData *rel, RID id, Record *record)`

- **Description**: Retrieves a record from storage by its Record ID.
- **Purpose**: Fetches a record for reading or updating.
- **Returns**: `RC_OK` if successful.

### **3.4 Scanning Functions**

#### `RC startScan(RM_TableData *rel, RM_ScanHandle *scan, Expr *cond)`

- **Description**: Initializes a scan operation on the table.
- **Purpose**: Allows filtering records based on a condition.
- **Returns**: `RC_OK` if successful.

#### `RC next(RM_ScanHandle *scan, Record *record)`

- **Description**: Retrieves the next matching record in a scan.
- **Purpose**: Iterates over matching records.
- **Returns**: `RC_OK` if a record is found, `RC_RM_NO_MORE_TUPLES` otherwise.

#### `RC closeScan(RM_ScanHandle *scan)`

- **Description**: Closes the scan operation.
- **Purpose**: Frees resources used during scanning.
- **Returns**: `RC_OK` if successful.

### **3.5 Schema Management**

#### `int getRecordSize(Schema *schema)`

- **Description**: Computes the total size of a record based on its schema.
- **Purpose**: Helps determine storage requirements.
- **Returns**: Size of the record in bytes.

#### `Schema *createSchema(...)`

- **Description**: Creates a schema for defining a table’s structure.
- **Purpose**: Specifies attributes and keys.
- **Returns**: Pointer to the schema.

#### `RC freeSchema(Schema *schema)`

- **Description**: Frees allocated schema memory.
- **Purpose**: Prevents memory leaks.
- **Returns**: `RC_OK` if successful.

### **3.6 Attribute Management**

#### `RC getAttr(Record *record, Schema *schema, int attrNum, Value **value)`

- **Description**: Retrieves an attribute’s value from a record.
- **Purpose**: Reads a specific attribute.
- **Returns**: `RC_OK` if successful.

#### `RC setAttr(Record *record, Schema *schema, int attrNum, Value *value)`

- **Description**: Sets an attribute’s value in a record.
- **Purpose**: Modifies a record’s attribute.
- **Returns**: `RC_OK` if successful.

---

## **4. MEMORY MANAGEMENT FUNCTIONS**

- `void *returnNull()`: Returns a NULL pointer.
- `int allocateSchemaMemoryFlag(int flag)`: Allocates or deallocates schema memory.
- `void initializeSchema()`: Initializes schema attributes.

---

## **5. UTILITY FUNCTIONS**

- `RC checkIfFileExists(char *fileName)`: Checks if a table file exists.
- `RC openAndCloseTableFile(char *fileName)`: Opens and closes a table file.
- `void markDirtyAndUnpin(BM_BufferPool *bufferPool, BM_PageHandle *page)`: Marks a page as dirty, unpins it, and forces it to disk.

---

## **6. TEST CASES**

### Implemented Tests:

- **Expression Evaluation Tests**
- **Record Manager Tests**
- **Scan Tests**

---

## **7. CONCLUSION**

The Record Manager efficiently handles tables and records with CRUD operations, scanning, and schema management while interacting with Buffer and Storage Managers.

Link to Video https://www.loom.com/share/12cb88847a7f4e8486d2a98727769310
