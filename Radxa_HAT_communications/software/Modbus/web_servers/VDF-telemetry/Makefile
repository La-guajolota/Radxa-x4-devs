# Makefile for VDF Telemetry
# Author: Adrián Silva Palafox

# Compiler variables
CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99 -I/usr/include/modbus
LIBS = -lmodbus -lrt

# Project variables
TARGET = vdf_telemetry
SOURCES = main.c
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
	sudo apt install -y libmodbus-dev build-essential
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
