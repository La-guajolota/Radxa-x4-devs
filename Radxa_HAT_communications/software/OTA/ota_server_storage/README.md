# OTA Firmware Update Server

This project provides a complete and secure Over-The-Air (OTA) firmware update server for embedded devices. It's built with Python and Flask, and it includes a web interface for managing firmware, a REST API for devices to check for updates, and an example client to demonstrate how to interact with the server.

## Features

-   **Web Interface:** A simple web interface to upload, view, and manage firmware versions for different device types.
-   **REST API:** A comprehensive REST API for devices to:
    -   Check for available updates.
    -   Download firmware binaries.
    -   Get information about firmware versions.
-   **HTTPS Support:** The server runs over HTTPS to ensure secure communication with devices.
-   **Firmware Database:** Firmware metadata is stored in a simple JSON file (`firmware_db.json`).
-   **Example Client:** A Python script (`ota_client_example.py`) is provided to demonstrate how a device can check for and download updates.
-   **Systemd Service:** A `ota-server.service` file is included for running the server as a systemd service in a production environment.

## Project Structure

```
.
├── ca_cert.pem
├── ca_key.pem
├── config/
├── firmware/
│   ├── dummyMCU_v2_Echidna.jpeg
│   ├── espS3_v1_blink.bin
│   └── espS3_v2_blink.bin
├── firmware_db.json
├── logs/
├── ota_client_example.py
├── ota_server.py
├── ota_setup.sh
├── ota-server.service
├── requirements.txt
└── start_ota_server.sh
```

-   `ota_server.py`: The main Flask application.
-   `ota_client_example.py`: An example of how a device would interact with the server.
-   `ota_setup.sh`: A script to set up the directory structure and initial files.
-   `start_ota_server.sh`: A script to start the server.
-   `ota-server.service`: A systemd service file for running the server as a service.
-   `requirements.txt`: A list of Python dependencies.
-   `firmware_db.json`: The database that stores firmware information.
-   `firmware/`: The directory where firmware files are stored.
-   `config/`: A directory for configuration files (currently unused).
-   `logs/`: A directory for log files (currently unused).
-   `ca_cert.pem` and `ca_key.pem`: SSL certificates for HTTPS.

## Getting Started

### Prerequisites

-   Python 3
-   pip

### Installation

1.  **Clone the repository:**

    ```bash
    git clone https://github.com/your-username/ota-server.git
    cd ota-server
    ```

2.  **Run the setup script:**

    The `ota_setup.sh` script will create the necessary directories and files.

    ```bash
    bash ota_setup.sh
    ```

3.  **Install dependencies:**

    ```bash
    pip3 install -r requirements.txt
    ```

4.  **Generate SSL certificates:**

    You will need to generate your own SSL certificates (`ca_cert.pem` and `ca_key.pem`) and place them in the root directory of the project. For development purposes, you can create self-signed certificates using OpenSSL:

    ```bash
    openssl req -x509 -newkey rsa:4096 -keyout ca_key.pem -out ca_cert.pem -sha256 -days 365 -nodes
    ```

### Running the Server

To start the server, run the `start_ota_server.sh` script:

```bash
./start_ota_server.sh
```

The server will be accessible at `https://<your-server-ip>:8070`.

### Running as a Service (Optional)

To run the server as a systemd service, you will need to edit the `ota-server.service` file and replace `/path/to/your/ota/server` with the absolute path to the project directory. Then, you can copy the file to the systemd directory and start the service:

```bash
sudo cp ota-server.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl start ota-server
sudo systemctl enable ota-server
```

## Web Interface

The web interface allows you to manage firmware for your devices. You can access it by opening `https://<your-server-ip>:8070` in your web browser.

From the web interface, you can:

-   Upload new firmware for a specific device type and version.
-   View a list of all available firmware.
-   Download firmware files.

## API Endpoints

The following API endpoints are available:

-   `GET /api/firmware`: List all available firmware.
-   `GET /api/firmware/<device_type>`: List all firmware for a specific device type.
-   `GET /api/firmware/<device_type>/latest`: Get the latest firmware version for a specific device type.
-   `GET /api/firmware/<device_type>/<version>/info`: Get information about a specific firmware version.
-   `GET /api/firmware/<device_type>/<version>/download`: Download a specific firmware version.
-   `GET /api/check_update?device_type=<device_type>&current_version=<current_version>`: Check if an update is available for a device.

## Example Client

The `ota_client_example.py` script shows how a device can use the API to check for and download firmware updates.

To run the example client, you will need to edit the `SERVER_URL`, `DEVICE_TYPE`, and `CURRENT_VERSION` variables in the script. Then, you can run the script:

```bash
python3 ota_client_example.py
```

## Customization

-   **Secret Key:** Change the `SECRET_KEY` in `ota_server.py` to a secure value.
-   **Port:** The server runs on port 8070 by default. You can change this in the `app.run()` call in `ota_server.py`.
-   **Firmware Database:** The firmware database is a simple JSON file. You can easily modify it to add more fields or change the structure.
-   **Authentication:** The server does not currently implement any authentication for the API endpoints. You can add authentication to the API endpoints to restrict access to authorized devices.
