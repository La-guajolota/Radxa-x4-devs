# Makefile for Modbus RTU Slave Application
# Description: Builds the RTU slave application
# Author: Adrián Silva Palafox

# Compiler variables
CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99 -I/usr/include/modbus
LIBS = -lmodbus -lrt

# Project variables
TARGET = rtu-slave

# Source files
SOURCES = rtu-slave.c
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
