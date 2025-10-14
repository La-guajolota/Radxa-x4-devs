# Modbus TCP Web Server

This example demonstrates a web server that provides a user interface to control a Modbus TCP device. It's a great example of how to create a web-based HMI (Human-Machine Interface) for an industrial device.

## Components

-   **`modbus_tcp_web.c`**: The main application written in C. It uses:
    -   **Mongoose**: A lightweight, embeddable web server library to serve the web page and handle API requests.
    -   **libmodbus**: To communicate with the Modbus TCP device.
-   **`www/index.html`**: A single-page web application that provides the user interface. It uses JavaScript's `fetch` API to communicate with the web server's REST API.
-   **`modbus_server_simulator.py`**: A Python-based Modbus TCP server simulator. This is extremely useful for testing the application without a physical PLC.

## How it Works

1.  The C application (`modbus_server`) starts a web server on port 8000 and also connects to a Modbus TCP device.
2.  When you open `http://localhost:8000` in your browser, the server sends the `index.html` file.
3.  The JavaScript in `index.html` makes API calls to the server (e.g., `/api/run`, `/api/status`).
4.  The C application receives these API calls and translates them into Modbus commands, which are sent to the PLC.
5.  The UI periodically polls the `/api/status` endpoint to keep the displayed information up-to-date.

## Dependencies

-   `libmodbus-dev`: For Modbus communication.
-   `build-essential`: For compiling the C code.
-   `python3` and `pip`: For the simulator.
-   `pymodbus`: The Python library for the simulator.

You can install the C dependencies with:
```bash
sudo apt-get update
sudo apt-get install libmodbus-dev build-essential
```
And the Python dependencies with:
```bash
pip install pymodbus
```

## How to Run

1.  **Start the Simulator (Optional but Recommended)**:
    In a separate terminal, start the Modbus TCP simulator. This will create a virtual PLC to connect to.
    ```bash
    python3 modbus_server_simulator.py
    ```
    The simulator will run on port 5020. You may need to adjust the `SERVER_IP` in `modbus_tcp_web.c` to `127.0.0.1` if you are running both on the same machine.

2.  **Compile the Web Server**:
    Use the provided `Makefile` to compile the C application.
    ```bash
    make
    ```

3.  **Run the Web Server**:
    ```bash
    ./modbus_server
    ```

4.  **Open the Web Interface**:
    Open your web browser and navigate to `http://<your_radxa_ip>:8000`. You should see the control panel and be able to interact with the (simulated) PLC.

## API Endpoints

The web server provides the following REST API endpoints:

-   `POST /api/run`: Toggles the RUN/STOP state.
-   `POST /api/dir`: Toggles the FWD/REV direction.
-   `POST /api/freq`: Sets the frequency. Expects a `freq` parameter in the body (e.g., `freq=50`).
-   `GET /api/status`: Returns the current status (frequency, run state, direction) in JSON format.
