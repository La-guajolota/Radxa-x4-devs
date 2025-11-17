# Makefile for DELTA M300 VFD RTU TUI Application
# Description: Builds the TUI application for monitoring and controlling the DELTA M300 VFD RTU
# Author: Adrián Silva Palafox

# Compiler variables
CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99 -I/usr/include/modbus -I/usr/include/ncurses
LIBS = -lmodbus -lrt -lncurses

# Project variables
TARGET = delta_m300_vfd_rtu_tui

# Added modular source files
SOURCES = main.c vfd_driver.c tui_display.c
OBJECTS = $(SOURCES:.c=.o)

# Default rule
all: $(TARGET)

# Build main executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)
	@echo "✅ Build successful: $(TARGET)"

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean generated files
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "🧹 Build files removed"

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt update	
	sudo apt install -y libmodbus-dev build-essential libncurses-dev
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