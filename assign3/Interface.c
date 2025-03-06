#include <stdio.h>
#include <stdlib.h>
#include "dberror.h"
#include "expr.h"
#include "record_mgr.h"
#include "tables.h"
#include "test_helper.h"

const char* DataTypes[] = {"Integer", "String", "Float", "BoolValue"};

int main()
{
    Record *r;
    printf("\n\t\tInterface for DB\n");
    char tempVariableString[50];
    char tableName[256]; // Fixed-size array for table name
    int tempVariableInt;
    int *keys, *columnSizes;
    Value *value;
    bool tempVariableBool;
    int choice, num, keynum = 0, i, dtChoice, keyChoice;
    char **names, *datatypes;
    Schema *schema;
    RM_TableData table;
    float tempVariableFloat;
    DataType *dt;

    initRecordManager(NULL);

    while (1)
    {
        printf("\n");
        printf("\n1. Create new table\n");
        printf("\n");
        printf("2. Delete table\n");
        printf("\n");
        printf("3. Insert record into the table\n");
        printf("\n");
        printf("4. Delete record from the table\n");
        printf("\n");
        printf("5. Update record in the table\n");
        printf("\n");
        printf("6. Exit\n");
        printf("Enter your choice :  ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            printf("\nEnter table name:\n");
            scanf("%255s", tableName); // Limit table name to 255 characters

            printf("\nCreate schema of the table\n Enter number of attributes:");
            scanf("%d", &num);

            dt = (DataType *)malloc(sizeof(DataType) * num);
            names = (char **)malloc(sizeof(char *) * num);
            columnSizes = (int *)malloc(sizeof(int) * num);
            keys = (int *)malloc(sizeof(int) * 0);

            for (int i = 0; i < num; i++)
            {
                printf("\nEnter name of the column:\n");
                names[i] = (char *)malloc(256); // Limit attribute name to 255 characters
                scanf("%255s", names[i]);

                printf("\nSelect datatype of the column:\n");
                printf("\n1. Integer\n");
                printf("\n2. Float\n");
                printf("\n3. String\n");
                printf("\n4. Bool\n");
                scanf("%d", &dtChoice);

                if (dtChoice == 1)
                {
                    dt[i] = DT_INT;
                    columnSizes[i] = 0;
                }
                else if (dtChoice == 2)
                {
                    dt[i] = DT_FLOAT;
                    columnSizes[i] = 0;
                }
                else if (dtChoice == 3)
                {
                    dt[i] = DT_STRING;
                    printf("\n Enter size of the column:\n");
                    scanf("%d", &columnSizes[i]);
                }
                else if (dtChoice == 4)
                {
                    dt[i] = DT_BOOL;
                    columnSizes[i] = 0;
                }
            }

            char **cpNames = (char **)malloc(sizeof(char *) * num);
            for (int i = 0; i < num; i++)
            {
                cpNames[i] = names[i];
            }

            schema = createSchema(num, cpNames, dt, columnSizes, 0, NULL);
            printf("%s", serializeSchema(schema));
            createTable(tableName, schema);
        }
        else if (choice == 2)
        {
            printf("Enter table name to delete:\n");
            scanf("%255s", tableName);
            deleteTable(tableName);
        }
        else if (choice == 3)
        {
            openTable(&table, tableName);
            printf("%s", serializeSchema(table.schema));
            printf("Enter number of records to insert:\n");
            scanf("%d", &num);
            createRecord(&r, table.schema);

            for (int i = 0; i < num; i++)
            {
                for (int j = 0; j < table.schema->numAttr; j++)
                {
                    printf("\nEnter value for attribute %s with datatype %s \n", table.schema->attrNames[j], DataTypes[table.schema->dataTypes[j]]);

                    if (table.schema->dataTypes[j] == DT_INT)
                    {
                        scanf("%d", &tempVariableInt);
                        MAKE_VALUE(value, DT_INT, tempVariableInt);
                    }
                    else if (table.schema->dataTypes[j] == DT_FLOAT)
                    {
                        scanf("%f", &tempVariableFloat);
                        MAKE_VALUE(value, DT_FLOAT, tempVariableFloat);
                    }
                    else if (table.schema->dataTypes[j] == DT_STRING)
                    {
                        scanf("%s", tempVariableString);
                        MAKE_STRING_VALUE(value, tempVariableString);
                    }
                    else if (table.schema->dataTypes[j] == DT_BOOL)
                    {
                        scanf("%d", &tempVariableBool);
                        MAKE_VALUE(value, DT_BOOL, tempVariableBool);
                    }

                    setAttr(r, table.schema, j, value);
                    insertRecord(&table, r);
                    freeVal(value);
                }
            }
        }
        else if (choice == 4)
        {
            printf("Enter number of records to delete:\n");
            scanf("%d", &num);
            createRecord(&r, table.schema);

            printf("\nSelect attribute based on which you want to delete a record:\n");
            for (int i = 0; i < table.schema->numAttr; i++)
            {
                printf("%d:%s\n", i + 1, table.schema->attrNames[i]);
            }
            scanf("%d", &choice);
            choice--;

            for (int i = 0; i < num; i++)
            {
                printf("\nEnter value of the attribute by which you want to delete a record:\n");

                if (table.schema->dataTypes[choice] == DT_INT)
                {
                    scanf("%d", &tempVariableInt);
                    MAKE_VALUE(value, DT_INT, tempVariableInt);
                }
                else if (table.schema->dataTypes[choice] == DT_FLOAT)
                {
                    scanf("%f", &tempVariableFloat);
                    MAKE_VALUE(value, DT_FLOAT, tempVariableFloat);
                }
                else if (table.schema->dataTypes[choice] == DT_STRING)
                {
                    scanf("%s", tempVariableString);
                    MAKE_STRING_VALUE(value, tempVariableString);
                }
                else if (table.schema->dataTypes[choice] == DT_BOOL)
                {
                    scanf("%d", &tempVariableBool);
                    MAKE_VALUE(value, DT_BOOL, tempVariableBool);
                }

                setAttr(r, table.schema, choice, value);
                deleteRecord(&table, r->id);
                freeVal(value);
            }
        }
        else if (choice == 5)
        {
            openTable(&table, tableName);
            printf("%s", serializeSchema(table.schema));
            printf("Enter number of records to update:\n");
            scanf("%d", &num);
            createRecord(&r, table.schema);

            printf("\nSelect attribute to update based on its index (0 to %d):\n", table.schema->numAttr - 1);
            int attributeIndex;
            scanf("%d", &attributeIndex);

            for (int i = 0; i < num; i++)
            {
                printf("\nEnter new value for the attribute with datatype %s: \n", DataTypes[table.schema->dataTypes[attributeIndex]]);

                if (table.schema->dataTypes[attributeIndex] == DT_INT)
                {
                    scanf("%d", &tempVariableInt);
                    MAKE_VALUE(value, DT_INT, tempVariableInt);
                }
                else if (table.schema->dataTypes[attributeIndex] == DT_FLOAT)
                {
                    scanf("%f", &tempVariableFloat);
                    MAKE_VALUE(value, DT_FLOAT, tempVariableFloat);
                }
                else if (table.schema->dataTypes[attributeIndex] == DT_STRING)
                {
                    scanf("%s", tempVariableString);
                    MAKE_STRING_VALUE(value, tempVariableString);
                }
                else if (table.schema->dataTypes[attributeIndex] == DT_BOOL)
                {
                    scanf("%d", &tempVariableBool);
                    MAKE_VALUE(value, DT_BOOL, tempVariableBool);
                }

                setAttr(r, table.schema, attributeIndex, value);
                updateRecord(&table, r);
                freeVal(value);
            }
        }
        else if (choice == 6)
        {
            printf("\nEnding Interface for DB!!!");
            shutdownRecordManager();
            exit(0);
        }
    }

    // Exit and cleanup
    printf("\nEnding Interface for DB!!!");
    shutdownRecordManager();
    exit(0);

    return 0;
}
