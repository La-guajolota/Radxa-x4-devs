# Modbus RTU Master TUI

This directory contains a Text-based User Interface (TUI) application for controlling and monitoring a Delta MS300 VFD (Variable Frequency Drive) over Modbus RTU.

## Functionality

This application provides a simple, terminal-based interface to:
- **Control the VFD**: Start, stop, and change the direction of the motor.
- **Adjust Speed**: Increase or decrease the target frequency.
- **Monitor Status**: View real-time telemetry from the VFD, including frequency, current, voltage, and RPM.

The application is built using the `ncurses` library for the TUI and `libmodbus` for Modbus communication.

## Files

- **`main.c`**: The main application entry point, containing the main loop.
- **`vfd_driver.c` / `vfd_driver.h`**: Handles all Modbus RTU communication.
- **`tui_display.c` / `tui_display.h`**: Manages the TUI, including drawing the interface and handling keyboard input.
- **`common.h`**: Contains shared data structures and constants.
- **`Makefile`**: The build script for the application.

## How to Use

### 1. Install Dependencies

You need to install `libmodbus-dev`, `libncurses-dev`, and `build-essential`:

```bash
make install-deps
```

### 2. Build the Application

To compile the TUI application, run:

```bash
make
```

This will create an executable file named `delta_m300_vfd_rtu_tui`.

### 3. Run the Application

Run the application with superuser privileges (if required for serial port access):

```bash
./delta_m300_vfd_rtu_tui
```

The application will try to connect to the VFD on `/dev/ttyS4`.

### 4. TUI Controls

- **`1`**: Toggle Start/Stop
- **`2`**: Toggle Forward/Reverse
- **Arrow Up/Down**: Adjust frequency by 1.0 Hz
- **Arrow Left/Right**: Adjust frequency by 0.1 Hz
- **`q`**: Quit the application

### 5. Clean Up

To remove the compiled files, run:

```bash
make clean
```
---
*This README was generated with the assistance of an AI.*