# RP2040 UART-to-RS485 Bridge

This project turns a Raspberry Pi Pico (RP2040) into a dedicated, high-performance UART-to-RS485 bridge. It is designed to connect a host computer (like a Radxa) to a Modbus RTU (RS485) bus.

## Functionality

The `hello_uart.c` program configures two UART peripherals on the RP2040 to create a seamless bridge:

- **`uart0`**: Connects to the host computer.
- **`uart1`**: Connects to an RS485 transceiver.

The bridge is **interrupt-driven**, which means it's highly efficient and doesn't waste CPU cycles polling for data. Data received on one UART is immediately forwarded to the other.

## Files

- **`hello_uart.c`**: The main C code for the UART bridge.
- **`CMakeLists.txt`**: The build configuration for the project using CMake and the Pico SDK.
- **`pico_sdk_import.cmake`**: A helper script to locate the Pico SDK.

## Hardware Setup

### Pinout

- **UART 0 (Host Interface)**
  - **GPIO 0 (TX)**: Connect to the RX pin of the host.
  - **GPIO 1 (RX)**: Connect to the TX pin of the host.
- **UART 1 (RS485 Interface)**
  - **GPIO 20 (TX)**: Connect to the DI (Data In) pin of the RS485 transceiver.
  - **GPIO 21 (RX)**: Connect to the RO (Data Out) pin of the RS485 transceiver.

### Baud Rate

The bridge is configured for a baud rate of **38400**, 8 data bits, no parity, and 1 stop bit (38400 8N1), which is suitable for the Modbus RTU communication in this project.

## How to Use

### 1. Setup the Build Environment

You need to have the Raspberry Pi Pico SDK, CMake, and the ARM GCC compiler installed. Follow the official [Pico getting started guide](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html) for instructions.

### 2. Build the Project

Create a `build` directory, run CMake, and then build the project:

```bash
mkdir build
cd build
cmake ..
make
```

This will generate a `hello_uart.uf2` file inside the `build` directory.

### 3. Flash the RP2040

1.  Hold down the **BOOTSEL** button on your Pico while connecting it to your computer via USB.
2.  It will appear as a mass storage device.
3.  Drag and drop the `hello_uart.uf2` file onto the Pico.

The Pico will reboot and start running the UART bridge program.
---
*This README was generated with the assistance of an AI.*
