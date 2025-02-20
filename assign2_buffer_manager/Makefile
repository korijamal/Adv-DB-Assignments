# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Executable targets
EXEC1 = test_case1
EXEC2 = test_case2

# Source files
SRC = buffer_mgr_stat.c buffer_mgr.c dberror.c storage_mgr.c
TEST_SRC1 = test_assign2_1.c
TEST_SRC2 = test_assign2_2.c

# Object files
OBJ = $(SRC:.c=.o)
OBJ1 = $(TEST_SRC1:.c=.o) $(OBJ)
OBJ2 = $(TEST_SRC2:.c=.o) $(OBJ)

# Default rule: Compile both test cases
all: $(EXEC1) $(EXEC2)

# Build the first test executable
$(EXEC1): $(OBJ1)
	$(CC) $(CFLAGS) -o $@ $(OBJ1)

# Build the second test executable
$(EXEC2): $(OBJ2)
	$(CC) $(CFLAGS) -o $@ $(OBJ2)

# Rule to compile all .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(EXEC1) $(EXEC2) *.o
