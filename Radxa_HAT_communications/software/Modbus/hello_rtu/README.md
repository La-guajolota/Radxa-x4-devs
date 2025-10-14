# Modbus RTU Client Example

This directory contains a simple example of a Modbus RTU client that communicates over a serial port.

## Files

- **`main.c`**: The main source code for the Modbus RTU client. It connects to a specific serial device, sets the slave ID, reads a couple of registers, and then disconnects.
- **`Makefile`**: A makefile to simplify compilation, dependency installation, and execution.
- **`modbus_rtu`**: The pre-compiled binary of the application.

## What is Modbus RTU?

Modbus RTU (Remote Terminal Unit) is a variant of the Modbus protocol that is used for serial communication. It is a master-slave protocol where the master sends a request to a slave, and the slave responds.

## Dependencies

To compile and run this example, you will need:
- `libmodbus`: A library for Modbus communication.

You can install it on a Debian-based system with:

```bash
sudo apt-get update
sudo apt-get install libmodbus-dev
```

The `Makefile` also provides a command to do this: `make install-deps`.

## How to Use

1.  **Configure the Serial Port**: In `main.c`, make sure the `device` variable is set to the correct serial port for your system (e.g., `/dev/ttyS4`, `/dev/ttyUSB0`, etc.). You can also adjust the baud rate and other serial parameters.

2.  **Compile the application**:
    You can use the provided `Makefile` to compile the application:

    ```bash
    make
    ```

3.  **Run the application**:
    Execute the compiled binary:

    ```bash
    ./modbus_rtu
    ```

    The program will attempt to connect to the serial device, read the registers from the specified slave, and print a success message if it works.

## Makefile Commands

The `Makefile` provides several useful commands:

- `make`: Compiles the project.
- `make clean`: Removes compiled files.
- `make install-deps`: Installs the required dependencies (for Debian-based systems).
- `make run`: Compiles and runs the application.
- `make help`: Shows a list of all available commands.
