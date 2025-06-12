# Compiler and flags
CC = g++

# Compiler flags (for compiling .cc -> .o)
CFLAGS = -Wall -Wextra -g -O0 -MMD -MP

# Linker flags (for linking the final binary)
LDFLAGS = -L/usr/local/lib

# Libraries to link
LDLIBS = -lelf++ -ldwarf++

# Directories
SRC_DIR := src
OBJ_DIR := build

# Find all .cc source files (recursively)
SRCS := $(shell find $(SRC_DIR) -name '*.cc')

# Convert source file paths to object file paths
OBJS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRCS:.cc=.o))
DEPS := $(OBJS:.o=.d)

# Target name
TARGET := mdbg

.PHONY: all clean debug

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
