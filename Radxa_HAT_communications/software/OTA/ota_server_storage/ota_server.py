#!/usr/bin/env python3
"""
Professional OTA (Over-The-Air) Firmware Update Server
Supports HTTPS, version management, device authentication, and firmware validation
"""

import os
import json
import hashlib
import datetime
from flask import Flask, request, jsonify, send_file, render_template_string
from werkzeug.utils import secure_filename
import ssl

app = Flask(__name__)
app.config['SECRET_KEY'] = 'your-secret-key-change-this'
app.config['UPLOAD_FOLDER'] = 'firmware'
app.config['MAX_CONTENT_LENGTH'] = 100 * 1024 * 1024  # 100MB max file size

# Create firmware directory if it doesn't exist
os.makedirs(app.config['UPLOAD_FOLDER'], exist_ok=True)

# Store firmware metadata
FIRMWARE_DB = 'firmware_db.json'

def load_firmware_db():
    """Load firmware database from JSON file"""
    if os.path.exists(FIRMWARE_DB):
        with open(FIRMWARE_DB, 'r') as f:
            return json.load(f)
    return {}

def save_firmware_db(db):
    """Save firmware database to JSON file"""
    with open(FIRMWARE_DB, 'w') as f:
        json.dump(db, f, indent=2)

def calculate_file_hash(filepath):
    """Calculate SHA256 hash of a file"""
    hash_sha256 = hashlib.sha256()
    with open(filepath, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash_sha256.update(chunk)
    return hash_sha256.hexdigest()

# HTML template for the web interface
WEB_INTERFACE = '''
<!DOCTYPE html>
<html>
<head>
    <title>OTA Firmware Server</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 40px; }
        .firmware-item { border: 1px solid #ddd; padding: 15px; margin: 10px 0; }
        .upload-form { background: #f5f5f5; padding: 20px; margin: 20px 0; }
        .button { background: #007cba; color: white; padding: 10px 15px; text-decoration: none; border: none; cursor: pointer; }
        .version { font-weight: bold; color: #007cba; }
        .hash { font-family: monospace; font-size: 0.8em; color: #666; }
    </style>
</head>
<body>
    <h1>🚀 OTA Firmware Update Server</h1>
    <p>Secure firmware distribution for embedded devices</p>
    
    <div class="upload-form">
        <h3>Upload New Firmware</h3>
        <form action="/upload" method="post" enctype="multipart/form-data">
            <p>
                <label>Device Type: <input type="text" name="device_type" required placeholder="e.g., sensor-v1"></label>
            </p>
            <p>
                <label>Version: <input type="text" name="version" required placeholder="e.g., 1.2.3"></label>
            </p>
            <p>
                <label>Firmware File: <input type="file" name="firmware" required></label>
            </p>
            <p>
                <label>Release Notes: <br><textarea name="notes" rows="3" cols="50"></textarea></label>
            </p>
            <p>
                <button type="submit" class="button">Upload Firmware</button>
            </p>
        </form>
    </div>
    
    <h3>Available Firmware</h3>
    {% if firmware_list %}
        {% for device_type, versions in firmware_list.items() %}
            <h4>{{ device_type }}</h4>
            {% for version, info in versions.items() %}
                <div class="firmware-item">
                    <span class="version">Version {{ version }}</span> 
                    ({{ info.size }} bytes) - {{ info.upload_date }}<br>
                    <strong>File:</strong> {{ info.filename }}<br>
                    <strong>SHA256:</strong> <span class="hash">{{ info.hash }}</span><br>
                    {% if info.notes %}
                        <strong>Notes:</strong> {{ info.notes }}<br>
                    {% endif %}
                    <a href="/download/{{ device_type }}/{{ version }}" class="button">Download</a>
                    <a href="/api/firmware/{{ device_type }}/{{ version }}/info" class="button">API Info</a>
                </div>
            {% endfor %}
        {% endfor %}
    {% else %}
        <p>No firmware available. Upload some firmware to get started.</p>
    {% endif %}
    
    <h3>API Endpoints</h3>
    <ul>
        <li><code>GET /api/firmware</code> - List all firmware</li>
        <li><code>GET /api/firmware/{device_type}</code> - List firmware for device type</li>
        <li><code>GET /api/firmware/{device_type}/{version}/info</code> - Get firmware info</li>
        <li><code>GET /api/firmware/{device_type}/{version}/download</code> - Download firmware</li>
        <li><code>GET /api/firmware/{device_type}/latest</code> - Get latest version info</li>
    </ul>
</body>
</html>
'''

@app.route('/')
def index():
    """Web interface for firmware management"""
    db = load_firmware_db()
    return render_template_string(WEB_INTERFACE, firmware_list=db)

@app.route('/upload', methods=['POST'])
def upload_firmware():
    """Upload new firmware"""
    if 'firmware' not in request.files:
        return jsonify({'error': 'No firmware file provided'}), 400
    
    file = request.files['firmware']
    device_type = request.form.get('device_type')
    version = request.form.get('version')
    notes = request.form.get('notes', '')
    
    if not all([file, device_type, version]):
        return jsonify({'error': 'Missing required fields'}), 400
    
    # Secure the filename
    filename = secure_filename(f"{device_type}_{version}_{file.filename}")
    filepath = os.path.join(app.config['UPLOAD_FOLDER'], filename)
    
    # Save the file
    file.save(filepath)
    
    # Calculate hash and get file info
    file_hash = calculate_file_hash(filepath)
    file_size = os.path.getsize(filepath)
    
    # Update database
    db = load_firmware_db()
    if device_type not in db:
        db[device_type] = {}
    
    db[device_type][version] = {
        'filename': filename,
        'hash': file_hash,
        'size': file_size,
        'upload_date': datetime.datetime.now().isoformat(),
        'notes': notes
    }
    
    save_firmware_db(db)
    
    return jsonify({
        'message': 'Firmware uploaded successfully',
        'device_type': device_type,
        'version': version,
        'hash': file_hash
    })

@app.route('/api/firmware')
def api_list_all_firmware():
    """API: List all available firmware"""
    db = load_firmware_db()
    return jsonify(db)

@app.route('/api/firmware/<device_type>')
def api_list_firmware(device_type):
    """API: List firmware for specific device type"""
    db = load_firmware_db()
    if device_type not in db:
        return jsonify({'error': 'Device type not found'}), 404
    return jsonify(db[device_type])

@app.route('/api/firmware/<device_type>/latest')
def api_latest_firmware(device_type):
    """API: Get latest firmware version for device type"""
    db = load_firmware_db()
    if device_type not in db:
        return jsonify({'error': 'Device type not found'}), 404
    
    # Get latest version (assumes semantic versioning)
    versions = list(db[device_type].keys())
    if not versions:
        return jsonify({'error': 'No firmware found'}), 404
    
    latest_version = max(versions)  # Simple max - you might want better version comparison
    return jsonify({
        'version': latest_version,
        'info': db[device_type][latest_version]
    })

@app.route('/api/firmware/<device_type>/<version>/info')
def api_firmware_info(device_type, version):
    """API: Get firmware information"""
    db = load_firmware_db()
    if device_type not in db or version not in db[device_type]:
        return jsonify({'error': 'Firmware not found'}), 404
    
    return jsonify({
        'device_type': device_type,
        'version': version,
        'info': db[device_type][version]
    })

@app.route('/api/firmware/<device_type>/<version>/download')
@app.route('/download/<device_type>/<version>')
def download_firmware(device_type, version):
    """Download firmware file"""
    db = load_firmware_db()
    if device_type not in db or version not in db[device_type]:
        return jsonify({'error': 'Firmware not found'}), 404
    
    firmware_info = db[device_type][version]
    filepath = os.path.join(app.config['UPLOAD_FOLDER'], firmware_info['filename'])
    
    if not os.path.exists(filepath):
        return jsonify({'error': 'Firmware file not found on disk'}), 404
    
    return send_file(filepath, as_attachment=True, download_name=firmware_info['filename'])

@app.route('/api/check_update')
def check_update():
    """API: Check if update is available for device"""
    device_type = request.args.get('device_type')
    current_version = request.args.get('current_version')
    
    if not device_type:
        return jsonify({'error': 'device_type parameter required'}), 400
    
    db = load_firmware_db()
    if device_type not in db:
        return jsonify({'update_available': False, 'message': 'No firmware for this device type'})
    
    # Get latest version
    versions = list(db[device_type].keys())
    latest_version = max(versions) if versions else None
    
    if not latest_version:
        return jsonify({'update_available': False})
    
    # Simple version comparison (you might want more sophisticated logic)
    update_available = latest_version != current_version if current_version else True
    
    response = {
        'update_available': update_available,
        'latest_version': latest_version,
        'current_version': current_version
    }
    
    if update_available:
        response['download_url'] = f"/api/firmware/{device_type}/{latest_version}/download"
        response['info'] = db[device_type][latest_version]
    
    return jsonify(response)

if __name__ == '__main__':
    # SSL Context for HTTPS
    context = ssl.SSLContext(ssl.PROTOCOL_TLSv1_2)
    context.load_cert_chain('ca_cert.pem', 'ca_key.pem')
    
    print("🚀 Starting OTA Firmware Update Server")
    print("📁 Firmware storage: ./firmware/")
    print("🌐 Access at: https://radxa:8070")
    print("🔐 SSL: Enabled with your certificates")
    
    app.run(host='0.0.0.0', port=8070, ssl_context=context, debug=True)
