# 串口 Modbus RTU Client Example

This directory contains a simple example of a **Modbus RTU client** that communicates over a serial port, using the `libmodbus` library.

---

## 📖 About Modbus RTU

> **Modbus RTU (Remote Terminal Unit)** is a variant of the Modbus protocol used for serial communication. It is a master-slave protocol where a master device sends a request to a slave device, and the slave responds. It's commonly used in industrial applications for communication between devices.

---

## 📂 Files

-   **`main.c`**: The main source code for the Modbus RTU client.
-   **`Makefile`**: A comprehensive makefile to simplify compilation, dependency installation, and execution.
-   **`modbus_rtu`**: The pre-compiled binary of the application.

---

## 🚀 Getting Started

### 1. Dependencies

You will need `libmodbus`. You can install it on a Debian-based system using the `Makefile` command:
```bash
make install-deps
```
Or manually:
```bash
sudo apt-get update
sudo apt-get install libmodbus-dev
```

### 2. Configuration

Before compiling, open `main.c` and configure the serial port parameters:
-   `device`: Set to your serial port (e.g., `/dev/ttyS4`, `/dev/ttyUSB0`).
-   `baud`, `parity`, `data_bit`, `stop_bit`: Adjust as needed for your device.
-   `REMOTE_ID`: Set the slave ID of the device you want to communicate with.

### 3. Compile and Run

-   **Compile the application**:
    ```bash
    make
    ```
-   **Run the application**:
    ```bash
    ./modbus_rtu
    ```
    > The program will attempt to connect to the serial device, read registers from the specified slave, and print a success message.

---

## 🛠️ Makefile Commands

The `Makefile` provides several useful commands:

| Command             | Description                                       |
| ------------------- | ------------------------------------------------- |
| `make`              | Compiles the project.                             |
| `make clean`        | Removes compiled files.                           |
| `make install-deps` | Installs required dependencies (Debian-based).    |
| `make run`          | Compiles and runs the application.                |
| `make help`         | Shows a list of all available commands.           |