# CS 525 - Advanced Database Organization  
## Programming Assignment 4  

**Group 17:**
- **Kori Kamoljonov** – A20594489 – 33.3% Contributed  
- **Chaitanya Datta** – A20568393 – 33.3% Contributed  
- **Vamshi Krishna** – A20582646 – 33.3% Contributed  

---

## README – B+ Tree Manager  

This file serves as a complete guide to the B+ Tree Manager's codebase. It explains key aspects such as initializing the B+ Tree, handling nodes, scanning the tree, and managing memory. The implementation relies on structured arrays to organize and manage node data, associated metadata, and scanning workflows effectively.

---

## B+ Tree Index Manager Documentation

This guide offers in-depth descriptions of the various functions implemented in the B+ Tree Index Manager. It enables the creation, modification, and querying of B+ Tree indexes—specialized data structures frequently used in database systems to enhance the efficiency of search and retrieval operations.

---

## Initialization and Shutdown

### `initIndexManager(void *mgmtData)`

**Description:**  
Initializes the Index Manager in preparation for B+ Tree operations. It sets up any required internal structures or configuration states.

**Parameters:**  
- `mgmtData`: Pointer to optional management data. Can be set to NULL if not applicable.

**Returns:**  
- A return code (RC) indicating whether the initialization was successful (`RC_OK`) or if an error occurred.

---

### `shutdownIndexManager()`

**Description:**  
Cleans up and shuts down the Index Manager, releasing any memory or resources used during its operation.

**Returns:**  
- A return code (`RC_OK`) or an appropriate error code in case of failure.

---

## B+ Tree Creation and Management

### `createBtree(char *idxId, DataType keyType, int n)`

**Description:**  
Initializes a new B+ Tree index using the provided identifier, key type, and node capacity.

**Parameters:**  
- `idxId`: A unique name for the index (e.g., a file name).  
- `keyType`: Specifies the type of keys stored in the index (e.g., `DT_INT`, `DT_STRING`).  
- `n`: Defines the maximum number of keys per node (i.e., the tree's order).

**Returns:**  
- A return code (`RC_OK`) indicating success or an error.

---

### `openBtree(BTreeHandle **tree, char *idxId)`

**Description:**  
Loads an existing B+ Tree index into memory, making it accessible for operations.

**Parameters:**  
- `tree`: Double pointer to a `BTreeHandle` structure.  
- `idxId`: Identifier of the index to be opened.

**Returns:**  
- An RC value (`RC_OK`) or error code.

---

### `closeBtree(BTreeHandle *tree)`

**Description:**  
Closes a previously opened B+ Tree and releases any associated memory or resources.

**Parameters:**  
- `tree`: Pointer to the `BTreeHandle`.

**Returns:**  
- A return code (`RC_OK`).

---

### `deleteBtree(char *idxId)`

**Description:**  
Removes the specified B+ Tree index and deletes all data related to it.

**Parameters:**  
- `idxId`: Identifier of the index to be removed.

**Returns:**  
- `RC_OK` or an error code if the operation fails.

---

## Accessing B+ Tree Information

### `getNumNodes(BTreeHandle *tree, int *result)`

**Description:**  
Returns the total count of nodes currently present in the B+ Tree structure.

**Parameters:**  
- `tree`: Pointer to the `BTreeHandle`.  
- `result`: Pointer to an integer where the node count will be stored.

**Returns:**  
- `RC_OK` or error code.

---

### `getNumEntries(BTreeHandle *tree, int *result)`

**Description:**  
Fetches the total number of key entries contained within the B+ Tree.

**Parameters:**  
- `tree`: Pointer to the `BTreeHandle`.  
- `result`: Pointer to an integer to store entry count.

**Returns:**  
- `RC_OK` or error code.

---

### `getKeyType(BTreeHandle *tree, DataType *result)`

**Description:**  
Determines the data type of the keys used in the B+ Tree index.

**Parameters:**  
- `tree`: Pointer to `BTreeHandle`.  
- `result`: Pointer to a `DataType` variable.

**Returns:**  
- `RC_OK` or error code.

---

## Index Access Operations

### `findKey(BTreeHandle *tree, Value *key, RID *result)`

**Description:**  
Searches the B+ Tree for a specific key and returns the corresponding `RID` if the key is found.

**Parameters:**  
- `tree`: Pointer to the tree.  
- `key`: Pointer to a `Value` structure.  
- `result`: Pointer to an `RID` structure.

**Returns:**  
- `RC_OK`, `RC_IM_KEY_NOT_FOUND`, or error.

---

### `insertKey(BTreeHandle *tree, Value *key, RID rid)`

**Description:**  
Adds a new key and its associated `RID` into the B+ Tree index.

**Parameters:**  
- `tree`: Pointer to tree structure.  
- `key`: Pointer to key.  
- `rid`: Record identifier.

**Returns:**  
- `RC_OK`, `RC_IM_KEY_ALREADY_EXISTS`, or error.

---

### `deleteKey(BTreeHandle *tree, Value *key)`

**Description:**  
Removes a key and its corresponding `RID` from the B+ Tree.

**Parameters:**  
- `tree`: Pointer to tree.  
- `key`: Pointer to key.

**Returns:**  
- `RC_OK`, `RC_IM_KEY_NOT_FOUND`, or error.

---

## Tree Scanning

### `openTreeScan(BTreeHandle *tree, BT_ScanHandle **handle)`

**Description:**  
Initializes a scan on the B+ Tree, enabling sequential access to entries.

**Parameters:**  
- `tree`: Pointer to the tree.  
- `handle`: Double pointer to a `BT_ScanHandle`.

**Returns:**  
- `RC_OK` or error.

---

### `nextEntry(BT_ScanHandle *handle, RID *result)`

**Description:**  
Retrieves the next available entry in an active scan.

**Parameters:**  
- `handle`: Pointer to scan handle.  
- `result`: Pointer to an `RID`.

**Returns:**  
- `RC_OK`, `RC_IM_NO_MORE_ENTRIES`, or error.

---

### `closeTreeScan(BT_ScanHandle *handle)`

**Description:**  
Ends the scan and releases any resources.

**Parameters:**  
- `handle`: Pointer to scan handle.

**Returns:**  
- `RC_OK` or error.

---

## Debug and Test Function

### `printTree(BTreeHandle *tree)`

**Purpose:**  
Generates a readable string representation of the B+ Tree structure. This helps with debugging and testing.

**Parameters:**  
- `tree`: Pointer to the `BTreeHandle`.

**Returns:**  
- `char *` – a dynamically allocated string describing the tree layout.  
> ⚠️ Caller is responsible for freeing this memory.

---
---

## 🧪 How to Run

To compile and test the B+ Tree Manager:

```bash
make clean
make
./test_assign4_1


Video Link - https://www.loom.com/share/c0cbb8d3a8d64a2e9311a1326d8fda42
