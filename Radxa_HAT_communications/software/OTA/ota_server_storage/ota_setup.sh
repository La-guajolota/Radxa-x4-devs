#!/bin/bash
# OTA Server Setup Script
# Sets up a complete OTA firmware update server with HTTPS

echo "🚀 Setting up OTA Firmware Update Server"

# Install required Python packages
echo "📦 Installing Python dependencies..."
pip3 install flask werkzeug

# Create directory structure
echo "📁 Creating directory structure..."
mkdir -p firmware
mkdir -p logs
mkdir -p config

# Create requirements.txt
cat > requirements.txt << 'EOF'
flask>=2.0.0
werkzeug>=2.0.0
EOF

# Create systemd service file (optional, for production)
cat > ota-server.service << 'EOF'
[Unit]
Description=OTA Firmware Update Server
After=network.target

[Service]
Type=simple
User=nobody
WorkingDirectory=/path/to/your/ota/server
ExecStart=/usr/bin/python3 ota_server.py
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
EOF

# Create simple client example
cat > ota_client_example.py << 'EOF'
#!/usr/bin/env python3
"""
Example OTA Client - Shows how devices would check for and download updates
"""
import requests
import hashlib
import json
from urllib3.exceptions import InsecureRequestWarning

# Suppress SSL warnings for self-signed certificates
requests.urllib3.disable_warnings(InsecureRequestWarning)

class OTAClient:
    def __init__(self, server_url, device_type, current_version):
        self.server_url = server_url
        self.device_type = device_type
        self.current_version = current_version
    
    def check_for_updates(self):
        """Check if firmware update is available"""
        try:
            response = requests.get(
                f"{self.server_url}/api/check_update",
                params={
                    'device_type': self.device_type,
                    'current_version': self.current_version
                },
                verify=False,  # Skip SSL verification for self-signed cert
                timeout=10
            )
            
            if response.status_code == 200:
                return response.json()
            else:
                print(f"Error checking for updates: {response.status_code}")
                return None
                
        except requests.RequestException as e:
            print(f"Network error: {e}")
            return None
    
    def download_firmware(self, version):
        """Download firmware update"""
        try:
            response = requests.get(
                f"{self.server_url}/api/firmware/{self.device_type}/{version}/download",
                verify=False,
                stream=True,
                timeout=30
            )
            
            if response.status_code == 200:
                filename = f"{self.device_type}_{version}_firmware.bin"
                
                with open(filename, 'wb') as f:
                    for chunk in response.iter_content(chunk_size=8192):
                        f.write(chunk)
                
                print(f"✅ Downloaded firmware: {filename}")
                return filename
            else:
                print(f"❌ Download failed: {response.status_code}")
                return None
                
        except requests.RequestException as e:
            print(f"❌ Download error: {e}")
            return None
    
    def verify_firmware(self, filepath, expected_hash):
        """Verify downloaded firmware hash"""
        hash_sha256 = hashlib.sha256()
        with open(filepath, "rb") as f:
            for chunk in iter(lambda: f.read(4096), b""):
                hash_sha256.update(chunk)
        
        actual_hash = hash_sha256.hexdigest()
        return actual_hash == expected_hash

# Example usage
if __name__ == "__main__":
    # Configure your device
    SERVER_URL = "https://radxa:8070"
    DEVICE_TYPE = "sensor-v1"
    CURRENT_VERSION = "1.0.0"
    
    client = OTAClient(SERVER_URL, DEVICE_TYPE, CURRENT_VERSION)
    
    print(f"🔍 Checking for updates for {DEVICE_TYPE} v{CURRENT_VERSION}")
    
    update_info = client.check_for_updates()
    
    if update_info and update_info.get('update_available'):
        print(f"🆕 Update available: v{update_info['latest_version']}")
        
        # Download the update
        firmware_file = client.download_firmware(update_info['latest_version'])
        
        if firmware_file:
            # Verify the download
            expected_hash = update_info['info']['hash']
            if client.verify_firmware(firmware_file, expected_hash):
                print("✅ Firmware verification successful!")
                print("🔄 Ready to install update...")
                # Here you would flash the firmware to your device
            else:
                print("❌ Firmware verification failed!")
    else:
        print("✅ No updates available - you're running the latest version!")
EOF

# Make files executable
chmod +x ota_client_example.py

# Create startup script
cat > start_ota_server.sh << 'EOF'
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
EOF

chmod +x start_ota_server.sh

echo "✅ OTA Server setup complete!"
echo ""
echo "📋 Next steps:"
echo "1. Make sure your SSL certificates (ca_cert.pem, ca_key.pem) are in this directory"
echo "2. Run: python3 ota_server.py"
echo "3. Access the web interface at: https://radxa:8070"
echo "4. Upload firmware files through the web interface"
echo "5. Test with the example client: python3 ota_client_example.py"
echo ""
echo "📁 Directory structure:"
echo "├── ota_server.py          # Main server application"
echo "├── ota_client_example.py  # Example client code"
echo "├── start_ota_server.sh    # Startup script"
echo "├── firmware/              # Firmware files storage"
echo "├── ca_cert.pem            # Your SSL certificate"
echo "└── ca_key.pem             # Your SSL private key"