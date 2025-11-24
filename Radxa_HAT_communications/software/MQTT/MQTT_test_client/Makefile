
# Makefile for MQTT Publisher Application
# Description: Builds the MQTT publisher example using Eclipse Paho C client
# Author: Adrian Silva Palafox

# Compiler variables
CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99
LIBS = -lpaho-mqtt3c

# Project variables
TARGET = mqtt_pub

# Source files
SOURCES = mqtt_pub.c
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
	sudo apt install -y libpaho-mqtt3c-dev build-essential
	@echo "📦 MQTT dependencies installed"
