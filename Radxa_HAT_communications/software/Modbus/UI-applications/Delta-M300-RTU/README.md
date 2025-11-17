# Delta M300 VFD Control Project

This project contains a set of applications for controlling a Delta MS300 Variable Frequency Drive (VFD) using Modbus RTU. It includes a master TUI application, a slave simulator, and a UART-to-RS485 bridge for the Raspberry Pi Pico.

## Project Structure

This project is divided into three main components:

- **`RTU-master-tui/`**: A Text-based User Interface (TUI) for controlling and monitoring the VFD.
- **`modbusRTU-slave/`**: A simulator that emulates a Delta MS300 VFD, useful for testing the master application without hardware.
- **`RP2040-uart-bridge/`**: Firmware for a Raspberry Pi Pico to act as a UART-to-RS485 bridge.

## Components

### 1. RTU Master TUI

A terminal-based application for controlling the VFD.

- **Language**: C
- **Libraries**: `ncurses`, `libmodbus`
- **For more details, see**: [`RTU-master-tui/README.md`](RTU-master-tui/README.md)

### 2. Modbus RTU Slave Simulator

A command-line application that simulates a VFD.

- **Language**: C
- **Libraries**: `libmodbus`
- **For more details, see**: [`modbusRTU-slave/README.md`](modbusRTU-slave/README.md)

### 3. RP2040 UART Bridge

Firmware for the Raspberry Pi Pico to bridge UART communication from a host computer to an RS485 bus.

- **Language**: C
- **SDK**: Raspberry Pi Pico SDK
- **For more details, see**: [`RP2040-uart-bridge/README.md`](RP2040-uart-bridge/README.md)

## How to Use

Each component has its own `README.md` file with detailed instructions on how to build and run the application. Please refer to the links above for more information.
---
*This README was generated with the assistance of an AI.*