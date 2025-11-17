# Modbus RTU Slave Simulator

This directory contains a Modbus RTU slave simulator for a Delta MS300 VFD (Variable Frequency Drive).

## Functionality

The `rtu-slave.c` application simulates the behavior of a VFD, making it possible to test Modbus master applications without needing physical hardware. It uses the `libmodbus` library to handle Modbus communication.

Key features:
- **Simulates a VFD:** Responds to Modbus commands to control a virtual motor.
- **Register Mapping:** Implements a register map compatible with the Delta MS300 VFD.
- **Dynamic Simulation:** Simulates motor ramp-up/down, frequency, RPM, current, and voltage.

## Files

- **`rtu-slave.c`**: The main source code for the slave simulator.
- **`Makefile`**: The build script for compiling the application.

## How to Use

### 1. Install Dependencies

First, you need to install `libmodbus-dev` and `build-essential`:

```bash
make install-deps
```

### 2. Build the Application

To compile the simulator, run:

```bash
make
```

This will create an executable file named `rtu-slave`.

### 3. Run the Simulator

You can run the simulator by providing a serial port as an argument. If you don't provide one, it will default to `/dev/ttyUSB1`.

```bash
./rtu-slave /dev/ttyUSB0
```

### 4. Clean Up

To remove the compiled files, run:

```bash
make clean
```
---
*This README was generated with the assistance of an AI.*