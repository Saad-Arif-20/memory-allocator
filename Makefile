# Makefile for Custom Memory Allocator
# Supports both Windows (MinGW) and Unix-like systems

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -I./include
CFLAGS_DEBUG = $(CFLAGS) -g -DDEBUG
CFLAGS_RELEASE = $(CFLAGS) -O2

# Directories
SRC_DIR = src
INC_DIR = include
TEST_DIR = tests
EXAMPLE_DIR = examples
BUILD_DIR = build

# Source files
SRCS = $(SRC_DIR)/memory_allocator.c $(SRC_DIR)/main.c
OBJS = $(BUILD_DIR)/memory_allocator.o $(BUILD_DIR)/main.o

# Output executable
TARGET = $(BUILD_DIR)/allocator_demo

# Test files
TEST_SRCS = $(TEST_DIR)/test_allocator.c
TEST_OBJS = $(BUILD_DIR)/test_allocator.o
TEST_TARGET = $(BUILD_DIR)/test_allocator

# Platform detection
ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    TEST_TARGET := $(TEST_TARGET).exe
    RM = del /Q
    MKDIR = if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
else
    RM = rm -f
    MKDIR = mkdir -p $(BUILD_DIR)
endif

# Example files
EXAMPLE_SRCS = $(EXAMPLE_DIR)/basic_usage.c $(EXAMPLE_DIR)/performance_test.c $(EXAMPLE_DIR)/string_processing.c
EXAMPLE_TARGETS = $(BUILD_DIR)/basic_usage.exe $(BUILD_DIR)/performance_test.exe $(BUILD_DIR)/string_processing.exe

# Default target
all: $(TARGET) examples

# Compile examples
examples: $(EXAMPLE_TARGETS)
	@echo Examples built successfully

$(BUILD_DIR)/basic_usage.exe: $(EXAMPLE_DIR)/basic_usage.c $(BUILD_DIR)/memory_allocator.o
	$(CC) $(CFLAGS_RELEASE) -o $@ $^

$(BUILD_DIR)/performance_test.exe: $(EXAMPLE_DIR)/performance_test.c $(BUILD_DIR)/memory_allocator.o
	$(CC) $(CFLAGS_RELEASE) -o $@ $^

$(BUILD_DIR)/string_processing.exe: $(EXAMPLE_DIR)/string_processing.c $(BUILD_DIR)/memory_allocator.o
	$(CC) $(CFLAGS_RELEASE) -o $@ $^

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compile main program
$(TARGET): $(BUILD_DIR) $(OBJS)
	$(CC) $(CFLAGS_RELEASE) -o $(TARGET) $(OBJS)
	@echo Build complete: $(TARGET)

# Compile object files
$(BUILD_DIR)/memory_allocator.o: $(SRC_DIR)/memory_allocator.c $(INC_DIR)/memory_allocator.h
	$(CC) $(CFLAGS_RELEASE) -c $(SRC_DIR)/memory_allocator.c -o $(BUILD_DIR)/memory_allocator.o

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c $(INC_DIR)/memory_allocator.h
	$(CC) $(CFLAGS_RELEASE) -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o

# Debug build
debug: CFLAGS_RELEASE = $(CFLAGS_DEBUG)
debug: clean $(TARGET)
	@echo Debug build complete

# Test build
test: $(BUILD_DIR) $(BUILD_DIR)/memory_allocator.o $(TEST_OBJS)
	$(CC) $(CFLAGS_RELEASE) -o $(TEST_TARGET) $(BUILD_DIR)/memory_allocator.o $(TEST_OBJS)
	@echo Test build complete: $(TEST_TARGET)

$(BUILD_DIR)/test_allocator.o: $(TEST_DIR)/test_allocator.c $(INC_DIR)/memory_allocator.h
	$(CC) $(CFLAGS_RELEASE) -c $(TEST_DIR)/test_allocator.c -o $(BUILD_DIR)/test_allocator.o

# Run the program
run: $(TARGET)
	./$(TARGET)

# Run tests
run-tests: test
	./$(TEST_TARGET)

# Clean build artifacts
clean:
	@rm -rf $(BUILD_DIR)
	@echo Clean complete

# Help
help:
	@echo Available targets:
	@echo   all        - Build the main program (default)
	@echo   debug      - Build with debug symbols
	@echo   test       - Build test program
	@echo   run        - Build and run main program
	@echo   run-tests  - Build and run tests
	@echo   clean      - Remove build artifacts
	@echo   help       - Show this help message

.PHONY: all debug test run run-tests clean help
