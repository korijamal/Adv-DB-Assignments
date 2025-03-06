CC = gcc
CFLAGS = -g -Wall

# List of source files
SRC_FILES = buffer_mgr_stat.c buffer_mgr.c dberror.c expr.c record_mgr.c rm_serializer.c storage_mgr.c
TEST_FILES = test_assign3_1.c test_expr.c Interface.c

# Object files
OBJ_FILES = $(SRC_FILES:.c=.o)
TEST_OBJ = $(TEST_FILES:.c=.o)

# Targets
all: test_assign3_1 test_expr Interface

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test_assign3_1: test_assign3_1.o $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

test_expr: test_expr.o $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

Interface: Interface.o $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o test_assign3_1 test_expr Interface

.PHONY: all clean
