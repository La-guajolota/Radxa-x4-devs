# 🔌 Modbus TCP Client Examples with libmodbus

This directory contains C examples demonstrating how to use the `libmodbus` library to communicate with a Modbus TCP server, such as a PLC.

---

## 📂 Files

-   **`hello_modbus.c`**: A basic test program that connects to a Modbus server, writes some register values, and then reads them back.
    > Great for a first connection test.

-   **`modbus_tcp_app.c`**: A simple, menu-driven command-line application to control a PLC.
    > Allows you to toggle RUN/STOP, FWD/REV, and set the frequency.

-   **`modbus_tcp_tui.c`**: A more advanced version of the control application that uses the `ncurses` library to create a Text-based User Interface (TUI).
    > Provides a more interactive way to control the PLC.

---

## 🚀 Getting Started

### 1. Dependencies

You will need `libmodbus` and `ncurses`. You can install them on a Debian-based system with:

```bash
sudo apt-get update
sudo apt-get install libmodbus-dev libncurses-dev
```

### 2. Configuration

Before compiling, open the `.c` files and change the `SERVER_IP` macro to the IP address of your Modbus TCP server.

### 3. Compile and Run

#### **Compile the applications:**

```bash
# For the simple test
gcc -o hello_modbus hello_modbus.c $(pkg-config --cflags --libs libmodbus)

# For the menu-driven app
gcc -o app_hello modbus_tcp_app.c $(pkg-config --cflags --libs libmodbus)

# For the TUI app
gcc -o app_tui modbus_tcp_tui.c $(pkg-config --cflags --libs libmodbus ncurses)
```

#### **Run the applications:**

```bash
./hello_modbus
# or
./app_hello
# or
./app_tui
```

---

## 📝 Notes

-   The `app_hello` and `app_tui` binaries are pre-compiled, but it's **highly recommended** to re-compile them after setting your server's IP.
-   The code simulates button presses by writing a `1` and then a `0` to the target register. This is a common technique for triggering actions on some PLCs.