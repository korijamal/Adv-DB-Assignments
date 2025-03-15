#include <stdio.h>
#include <stdlib.h>
#include "dberror.h"
#include "expr.h"
#include "record_mgr.h"
#include "tables.h"
#include "test_helper.h"

const char* DataTypeLabels[] = {"Integer", "String", "Float", "Boolean"};

int main()
{
    Record *recordPtr;
    printf("\n\t\tWelcome to the Storage Manager\n");
    char inputString[50];
    char tableIdentifier[256]; 
    int inputInt;
    int *primaryKeys, *columnDimensions;
    Value *valuePtr;
    bool inputBool;
    int userChoice, attributeCount, keyCount = 0, i, typeChoice, keySelection;
    char **attributeNames, *dataTypes;
    Schema *tableSchema;
    RM_TableData tableData;
    float inputFloat;
    DataType *dataTypeArray;

    initRecordManager(NULL);

    while (1)
    {
        printf("\n");
        printf("\n");
        printf("\n1. Create a new table\n");
        printf("\n");
        printf("2. Delete a table\n");
        printf("\n");
        printf("3. Insert a record into the table\n");
        printf("\n");
        printf("4. Delete a record from the table\n");
        printf("\n");
        printf("5. Update a record in the table\n");
        printf("\n");
        printf("6. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &userChoice);

        if (userChoice == 1) // Create Table
        {
            printf("\nEnter table name:\n");
            scanf("%255s", tableIdentifier); 

            printf("\nCreate schema of the table\n Enter number of attributes:");
            scanf("%d", &attributeCount);

            dataTypeArray = (DataType *)malloc(sizeof(DataType) * attributeCount);
            attributeNames = (char **)malloc(sizeof(char *) * attributeCount);
            columnDimensions = (int *)malloc(sizeof(int) * attributeCount);
            primaryKeys = (int *)malloc(sizeof(int) * 0);

            for (int i = 0; i < attributeCount; i++)
            {
                printf("\nEnter name of the column:\n");
                attributeNames[i] = (char *)malloc(256); // Limit attribute name to 255 characters
                scanf("%255s", attributeNames[i]);

                printf("\nSelect datatype of the column:\n");
                printf("\n1. Integer\n");
                printf("\n2. Float\n");
                printf("\n3. String\n");
                printf("\n4. Boolean\n");
                scanf("%d", &typeChoice);

                if (typeChoice == 1)
                {
                    dataTypeArray[i] = DT_INT;
                    columnDimensions[i] = 0;
                }
                else if (typeChoice == 2)
                {
                    dataTypeArray[i] = DT_FLOAT;
                    columnDimensions[i] = 0;
                }
                else if (typeChoice == 3)
                {
                    dataTypeArray[i] = DT_STRING;
                    printf("\n Enter size of the column:\n");
                    scanf("%d", &columnDimensions[i]);
                }
                else if (typeChoice == 4)
                {
                    dataTypeArray[i] = DT_BOOL;
                    columnDimensions[i] = 0;
                }
            }

            char **copiedNames = (char **)malloc(sizeof(char *) * attributeCount);
            for (int i = 0; i < attributeCount; i++)
            {
                copiedNames[i] = attributeNames[i];
            }

            tableSchema = createSchema(attributeCount, copiedNames, dataTypeArray, columnDimensions, 0, NULL);
            printf("%s", serializeSchema(tableSchema));
            createTable(tableIdentifier, tableSchema);

            if (createTable(tableIdentifier, tableSchema) == RC_OK)
                printf("Table '%s' created successfully!\n", tableIdentifier);
            else
                printf("Error: Failed to create table '%s'.\n", tableIdentifier);
        }
        else if (userChoice == 2) // Delete Table
        {
            printf("Enter table name to delete:\n");
            scanf("%255s", tableIdentifier);
            deleteTable(tableIdentifier);
            if (deleteTable(tableIdentifier) == RC_OK)
                printf("Table '%s' deleted successfully!\n", tableIdentifier);
            else
                printf("Error: Table '%s' does not exist.\n", tableIdentifier);
        }
        else if (userChoice == 3) // Inserting record into the table
        {
            openTable(&tableData, tableIdentifier);
            printf("%s", serializeSchema(tableData.schema));
            printf("Enter number of records to insert:\n");
            scanf("%d", &attributeCount);
            createRecord(&recordPtr, tableData.schema);

            for (int i = 0; i < attributeCount; i++)
            {
                for (int j = 0; j < tableData.schema->numAttr; j++)
                {
                    printf("\nEnter value for attribute %s with datatype %s \n", tableData.schema->attrNames[j], DataTypeLabels[tableData.schema->dataTypes[j]]);

                    if (tableData.schema->dataTypes[j] == DT_INT)
                    {
                        scanf("%d", &inputInt);
                        MAKE_VALUE(valuePtr, DT_INT, inputInt);
                    }
                    else if (tableData.schema->dataTypes[j] == DT_FLOAT)
                    {
                        scanf("%f", &inputFloat);
                        MAKE_VALUE(valuePtr, DT_FLOAT, inputFloat);
                    }
                    else if (tableData.schema->dataTypes[j] == DT_STRING)
                    {
                        scanf("%s", inputString);
                        MAKE_STRING_VALUE(valuePtr, inputString);
                    }
                    else if (tableData.schema->dataTypes[j] == DT_BOOL)
                    {
                        scanf("%d", &inputBool);
                        MAKE_VALUE(valuePtr, DT_BOOL, inputBool);
                    }

                    setAttr(recordPtr, tableData.schema, j, valuePtr);
                    insertRecord(&tableData, recordPtr);
                    freeVal(valuePtr);
                }  
            }
            printf("Records inserted successfully into '%s'!\n", tableIdentifier);
        }
        else if (userChoice == 4) // Delete record in the table
        {
            printf("Enter number of records to delete:\n");
            scanf("%d", &attributeCount);
            createRecord(&recordPtr, tableData.schema);

            printf("\nSelect attribute based on which you want to delete a record:\n");
            for (int i = 0; i < tableData.schema->numAttr; i++)
            {
                printf("%d:%s\n", i + 1, tableData.schema->attrNames[i]);
            }
            scanf("%d", &userChoice);
            userChoice--;

            for (int i = 0; i < attributeCount; i++)
            {
                printf("\nEnter value of the attribute by which you want to delete a record:\n");

                if (tableData.schema->dataTypes[userChoice] == DT_INT)
                {
                    scanf("%d", &inputInt);
                    MAKE_VALUE(valuePtr, DT_INT, inputInt);
                }
                else if (tableData.schema->dataTypes[userChoice] == DT_FLOAT)
                {
                    scanf("%f", &inputFloat);
                    MAKE_VALUE(valuePtr, DT_FLOAT, inputFloat);
                }
                else if (tableData.schema->dataTypes[userChoice] == DT_STRING)
                {
                    scanf("%s", inputString);
                    MAKE_STRING_VALUE(valuePtr, inputString);
                }
                else if (tableData.schema->dataTypes[userChoice] == DT_BOOL)
                {
                    scanf("%d", &inputBool);
                    MAKE_VALUE(valuePtr, DT_BOOL, inputBool);
                }

                setAttr(recordPtr, tableData.schema, userChoice, valuePtr);
                deleteRecord(&tableData, recordPtr->id);
                freeVal(valuePtr);
            }
        }
        else if (userChoice == 5) // Updating the table
        {
            openTable(&tableData, tableIdentifier);
            printf("%s", serializeSchema(tableData.schema));
            printf("Enter number of records to update:\n");
            scanf("%d", &attributeCount);
            createRecord(&recordPtr, tableData.schema);

            printf("\nSelect attribute to update based on its index (0 to %d):\n", tableData.schema->numAttr - 1);
            int attributeIndex;
            scanf("%d", &attributeIndex);

            for (int i = 0; i < attributeCount; i++)
            {
                printf("\nEnter new value for the attribute with datatype %s: \n", DataTypeLabels[tableData.schema->dataTypes[attributeIndex]]);

                if (tableData.schema->dataTypes[attributeIndex] == DT_INT)
                {
                    scanf("%d", &inputInt);
                    MAKE_VALUE(valuePtr, DT_INT, inputInt);
                }
                else if (tableData.schema->dataTypes[attributeIndex] == DT_FLOAT)
                {
                    scanf("%f", &inputFloat);
                    MAKE_VALUE(valuePtr, DT_FLOAT, inputFloat);
                }
                else if (tableData.schema->dataTypes[attributeIndex] == DT_STRING)
                {
                    scanf("%s", inputString);
                    MAKE_STRING_VALUE(valuePtr, inputString);
                }
                else if (tableData.schema->dataTypes[attributeIndex] == DT_BOOL)
                {
                    scanf("%d", &inputBool);
                    MAKE_VALUE(valuePtr, DT_BOOL, inputBool);
                }

                setAttr(recordPtr, tableData.schema, attributeIndex, valuePtr);
                updateRecord(&tableData, recordPtr);
                freeVal(valuePtr);
            }
        }
        else if (userChoice == 6)  // Exit and cleanup
        {
            printf("\nEnding Interface for DB!!!");
            shutdownRecordManager();
            exit(0);
        }
    }

    printf("\nEnding Interface for DB!!!");
    shutdownRecordManager();
    exit(0);

    return 0;
}