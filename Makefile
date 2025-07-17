# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic
LDFLAGS = -lm

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c) \
       $(wildcard $(SRC_DIR)/common/*.c) \
       $(wildcard $(SRC_DIR)/core/domain/*.c) \
       $(wildcard $(SRC_DIR)/core/usecases/*.c) \
       $(wildcard $(SRC_DIR)/adapters/file_system/*.c) \
       $(wildcard $(SRC_DIR)/adapters/parsers/*.c) \
       $(wildcard $(SRC_DIR)/infrastructure/cli/*.c) \
       $(wildcard $(SRC_DIR)/infrastructure/process/*.c)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Executable
TARGET = $(BIN_DIR)/dicotodir

# Default target
all: directories $(TARGET)

# Create directories
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/common
	@mkdir -p $(BUILD_DIR)/core/domain
	@mkdir -p $(BUILD_DIR)/core/usecases
	@mkdir -p $(BUILD_DIR)/adapters/file_system
	@mkdir -p $(BUILD_DIR)/adapters/parsers
	@mkdir -p $(BUILD_DIR)/infrastructure/cli
	@mkdir -p $(BUILD_DIR)/infrastructure/process
	@mkdir -p $(BIN_DIR)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Link object files
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run tests
test: all
	# Add test commands here

# Install
install: all
	cp $(TARGET) /usr/local/bin/

# Uninstall
uninstall:
	rm -f /usr/local/bin/dicotodir

.PHONY: all directories clean test install uninstall