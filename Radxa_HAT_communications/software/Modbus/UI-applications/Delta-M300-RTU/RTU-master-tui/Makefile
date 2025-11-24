# Makefile for DELTA M300 VFD RTU TUI Application
# Description: Builds the TUI application for monitoring and controlling the DELTA M300 VFD RTU
# Author: Adrián Silva Palafox

# Compiler variables
CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99 -Iinclude -I/usr/include/modbus -I/usr/include/ncurses -I/usr/include/paho-mqtt3c
LIBS = -lmodbus -lrt -lncurses -lpaho-mqtt3c

# Project variables
TARGET = delta_m300_vfd_rtu_tui

# Sources in src/, build objects into build/, binary in bin/
SOURCES = src/main.c src/vfd_driver.c src/tui_display.c src/mqtt_driver.c
OBJECTS = $(patsubst src/%.c, build/%.o, $(SOURCES))

BUILD_DIR = build
BIN_DIR = bin

# Default rule
all: $(BIN_DIR)/$(TARGET)

# Build main executable
$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJECTS) -o $(BIN_DIR)/$(TARGET) $(LIBS)
	@echo "✅ Build successful: $(BIN_DIR)/$(TARGET)"

# Compile object files from src/ into build/
$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean generated files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(TARGET) *.o
	@echo "🧹 Build files removed"

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt update	
	sudo apt install -y libmodbus-dev build-essential libncurses-dev libpaho-mqtt3c-dev
	@echo "📦 Dependencies installed"

# Run the application
run: $(TARGET)
	./$(TARGET)

# Show help
help:
	@echo "📖 Available commands:"
	@echo "  make              - Build the project"
	@echo "  make clean        - Clean build files"
	@echo "  make install-deps - Install system dependencies"
	@echo "  make run          - Build and run"
	@echo "  make help         - Show this help"

# Avoid conflicts with files of the same name
.PHONY: all clean install-deps run help