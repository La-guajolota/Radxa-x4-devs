# Makefile for Modbus RTU example as a client
# Author: Adrián Silva Palafox

# Compiler variables
CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99 -D_GNU_SOURCE -I/usr/include/modbus
LIBS = -lmodbus -lpthread
TARGET = modbus_rtu
SOURCES = main.c

# Directory variables
SRCDIR = .
BUILDDIR = build

# Object files
OBJECTS = $(SOURCES:%.c=$(BUILDDIR)/%.o)

# Default rule
all: $(TARGET)

# Create build directory if it doesn't exist
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Build main executable
$(TARGET): $(BUILDDIR) $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)
	@echo "✅ Build successful: $(TARGET)"

# Compile object files
$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean generated files
clean:
	rm -rf $(BUILDDIR)
	rm -f $(TARGET)
	@echo "🧹 Build files removed"

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt update
	sudo apt install -y libmodbus-dev libncurses-dev build-essential
	@echo "📦 Dependencies installed"

# Run the server
run: $(TARGET)
	./$(TARGET)

# Run in debug mode (without real Modbus)
run-debug: $(TARGET)
	@echo "🐛 Running in DEBUG mode"
	./$(TARGET)

# Check dependencies
check-deps:
	@echo "🔍 Checking dependencies..."
	@pkg-config --exists libmodbus && echo "✅ libmodbus found" || echo "❌ libmodbus NOT found"

# Optimized build for production
release: CFLAGS += -O2 -DNDEBUG
release: $(TARGET)
	@echo "🚀 Built for production"

# Show help
help:
	@echo "📖 Available commands:"
	@echo "  make              - Build the project"
	@echo "  make clean        - Clean build files"
	@echo "  make install-deps - Install system dependencies"
	@echo "  make run          - Build and run"
	@echo "  make run-debug    - Run in debug mode"
	@echo "  make debug        - Build with debug symbols"
	@echo "  make release      - Build optimized"
	@echo "  make check-deps   - Check dependencies"
	@echo "  make help         - Show this help"

# Avoid conflicts with files of the same name
.PHONY: all clean install-deps run run-debug check-deps debug release help