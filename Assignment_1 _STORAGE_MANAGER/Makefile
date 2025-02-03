.PHONY: all clean

# Default target to build the test executable
all: test_17

# Compile the test executable
test_17: test_assign1_1.c storage_mgr.c dberror.c
	gcc -Wall -Wextra -g -o test_17 test_assign1_1.c storage_mgr.c dberror.c

# Clean target to remove compiled files
clean:
	rm -f test_17
