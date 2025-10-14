# Modbus TCP Client Examples with libmodbus

This directory contains C examples demonstrating how to use the `libmodbus` library to communicate with a Modbus TCP server, such as a PLC.

## Files

- **`hello_modbus.c`**: A basic test program that connects to a Modbus server, writes some register values, and then reads them back. This is a great starting point to verify your connection.
- **`modbus_tcp_app.c`**: A simple, menu-driven command-line application to control a PLC. It allows you to:
    - Toggle RUN/STOP state.
    - Toggle FWD/REV direction.
    - Set the frequency.
- **`modbus_tcp_tui.c`**: A more advanced version of the control application that uses the `ncurses` library to create a Text-based User Interface (TUI). This provides a more interactive way to control the PLC.

## Dependencies

To compile and run these examples, you will need:
- `libmodbus`: A library for Modbus communication.
- `ncurses`: For the TUI application (`modbus_tcp_tui.c`).

You can typically install these on a Debian-based system like Raspberry Pi OS or Ubuntu with:

```bash
sudo apt-get update
sudo apt-get install libmodbus-dev libncurses-dev
```

## How to Compile and Run

1.  **Set the Server IP**: Before compiling, make sure to change the `SERVER_IP` macro in the C files to the IP address of your Modbus TCP server (e.g., your PLC).

2.  **Compile the applications**:

    ```bash
    # For the simple test
    gcc -o hello_modbus hello_modbus.c $(pkg-config --cflags --libs libmodbus)

    # For the menu-driven app
    gcc -o app_hello modbus_tcp_app.c $(pkg-config --cflags --libs libmodbus)

    # For the TUI app
    gcc -o app_tui modbus_tcp_tui.c $(pkg-config --cflags --libs libmodbus ncurses)
    ```

3.  **Run the applications**:

    ```bash
    ./hello_modbus
    ./app_hello
    ./app_tui
    ```

## Notes

- The `app_hello` and `app_tui` applications are pre-compiled in this directory, but it's recommended to re-compile them after setting your server's IP.
- The code simulates button presses by writing a `1` and then a `0` to the target register. This is a common way to trigger actions on some PLCs.
