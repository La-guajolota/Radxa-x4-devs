# 🌐 Modbus TCP Web Server

This example demonstrates a web server that provides a user interface to control a Modbus TCP device, effectively creating a web-based **HMI (Human-Machine Interface)**.

---

## 🧩 Components

-   **`modbus_tcp_web.c`**: The main C application that runs the web server and Modbus client.
    -   Uses **Mongoose** for the web server.
    -   Uses **libmodbus** for Modbus TCP communication.
-   **`www/index.html`**: A single-page web application that provides the user interface.
-   **`modbus_server_simulator.py`**: A Python-based Modbus TCP server simulator, perfect for testing without a real PLC.

---

## ⚙️ How it Works

1.  The C application (`modbus_server`) starts a web server on port **8000** and connects to a Modbus TCP device.
2.  When you open `http://localhost:8000` in your browser, the `index.html` file is served.
3.  The JavaScript in the HTML file makes API calls to the C application's web server.
4.  The C application translates these API calls into Modbus commands and sends them to the PLC.
5.  The web UI periodically polls the `/api/status` endpoint to stay synchronized with the device state.

---

## 🚀 Getting Started

### 1. Dependencies

-   **For the C application**:
    ```bash
    sudo apt-get update
    sudo apt-get install libmodbus-dev build-essential
    ```
-   **For the Python simulator**:
    ```bash
    pip install pymodbus
    ```

### 2. How to Run

#### **Step 1: Start the Simulator (Recommended)**
In a separate terminal, start the Modbus TCP simulator:
```bash
python3 modbus_server_simulator.py
```
> This creates a virtual PLC on port 5020. You may need to change the `SERVER_IP` in `modbus_tcp_web.c` to `127.0.0.1`.

#### **Step 2: Compile the Web Server**
Use the provided `Makefile`:
```bash
make
```

#### **Step 3: Run the Web Server**
```bash
./modbus_server
```

#### **Step 4: Open the Web Interface**
Open your web browser and navigate to `http://<your_radxa_ip>:8000`. You should see the control panel.

---

## 📡 API Endpoints

The web server provides the following REST API:

| Method | Endpoint       | Description                                            |
| ------ | -------------- | ------------------------------------------------------ |
| `POST` | `/api/run`     | Toggles the RUN/STOP state.                            |
| `POST` | `/api/dir`     | Toggles the FWD/REV direction.                         |
| `POST` | `/api/freq`    | Sets the frequency (e.g., `freq=50`).                  |
| `GET`  | `/api/status`  | Returns the current device status in JSON format.      |