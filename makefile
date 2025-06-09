# Compiler and flags
CC = g++
CFLAGS = -Wall -Wextra -g -O0 -MMD -MP

# Directories
SRC_DIR := src
OBJ_DIR := build

# Find all .cc source files (recursively)
SRCS := $(shell find $(SRC_DIR) -name '*.cc' ! -path '$(SRC_DIR)/testing/*')

# Convert each source file path to its matching object file path
OBJS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRCS:.cc=.o))
DEPS := $(OBJS:.o=.d)

# Target name
TARGET := mdbg

.PHONY: all clean debug

# Default target
all: $(TARGET)

# Link all object files into the final binary
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files into object files, preserving directory structure
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Include generated dependency files (if any)
-include $(DEPS)

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Debug rule to print flags
debug:
	@echo "CFLAGS: $(CFLAGS)"
	@echo "SRCS:"
	@for f in $(SRCS); do echo "  $$f"; done
	@echo "OBJS:"
	@for f in $(OBJS); do echo "  $$f"; done
