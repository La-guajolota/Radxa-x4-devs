# Makefile for Modbus TCP Web Server
# Author: Adrián Silva Palafox

# Compiler variables
CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99 -D_GNU_SOURCE -I/usr/include/modbus -I/usr/include/ncurses
LIBS = -lmodbus -lpthread
TARGET = modbus_server
SOURCES = modbus_tcp_web.c mongoose.c

# Directory variables
SRCDIR = .
BUILDDIR = build
WWWDIR = www

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

# Check if www directory exists
check-www:
	@if [ ! -d "$(WWWDIR)" ]; then \
		echo "⚠️  Creating directory $(WWWDIR)"; \
		mkdir -p $(WWWDIR); \
	fi
	@if [ ! -f "$(WWWDIR)/index.html" ]; then \
		echo "⚠️  Missing $(WWWDIR)/index.html"; \
	else \
		echo "✅ Web directory properly configured"; \
	fi

# Check dependencies
check-deps:
	@echo "🔍 Checking dependencies..."
	@pkg-config --exists libmodbus && echo "✅ libmodbus found" || echo "❌ libmodbus NOT found"
	@echo "🔍 Checking source files..."
	@[ -f "mongoose.h" ] && echo "✅ mongoose.h found" || echo "❌ mongoose.h NOT found"
	@[ -f "mongoose.c" ] && echo "✅ mongoose.c found" || echo "❌ mongoose.c NOT found"

# Build with debug info
debug: CFLAGS += -g -DDEBUG_WEB
debug: clean $(TARGET)
	@echo "🐛 Built with debug symbols and DEBUG_WEB mode"

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
	@echo "  make check-www    - Check web directory"
	@echo "  make help         - Show this help"

# Avoid conflicts with files of the same name
.PHONY: all clean install-deps run run-debug check-www check-deps debug release help