# Compiler and flags
COMPILER = gcc
FLAGS = -Wall -Wextra -g
BUILD_DIR = build

# Source files
SOURCES = main.c game.c player.c rooms.c creatures.c items.c commands.c
# Object files located in build/ directory
OBJECTS = $(BUILD_DIR)/main.o $(BUILD_DIR)/game.o $(BUILD_DIR)/player.o $(BUILD_DIR)/rooms.o $(BUILD_DIR)/creatures.o $(BUILD_DIR)/items.o $(BUILD_DIR)/commands.o

# Final executable name
OUTPUT = dungeon

# Default target
all: $(OUTPUT)

# Link object files to create the final executable
$(OUTPUT): $(OBJECTS)
	$(COMPILER) $(FLAGS) -o $(OUTPUT) $(OBJECTS)

# Rule to compile source files into object files in the build directory
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(COMPILER) $(FLAGS) -c $< -o $@

# Clean up generated files and folders
clean:
	rm -rf $(BUILD_DIR) $(OUTPUT)
