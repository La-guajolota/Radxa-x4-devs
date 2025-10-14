#!/bin/bash
# Start the OTA server

# Check if certificates exist
if [ ! -f "ca_cert.pem" ] || [ ! -f "ca_key.pem" ]; then
    echo "❌ SSL certificates not found!"
    echo "Please ensure ca_cert.pem and ca_key.pem are in the current directory"
    exit 1
fi

# Start the server
echo "🚀 Starting OTA Server..."
python3 ota_server.py
