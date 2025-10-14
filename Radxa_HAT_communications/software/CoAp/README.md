# CoAP: Constrained Application Protocol

This directory contains a Python-based CoAP server and client for demonstrating communication with simulated IoT sensors.

## Files

- **`demo_server.py`**: A CoAP server that simulates temperature, humidity, and pressure sensors.
- **`demo_cliente.py`**: A command-line client to connect to the server, read sensor data, and monitor it.

## What is CoAP?

CoAP is a specialized web transfer protocol for use with constrained nodes and constrained networks in the Internet of Things. The protocol is designed for machine-to-machine (M2M) applications such as smart energy and building automation.

## Dependencies

To run these examples, you need to install the `aiocoap` library:

```bash
pip install aiocoap
```

## How to Use

1.  **Start the Server**:
    Run the server on your Radxa board or any machine that will act as the sensor hub:

    ```bash
    python3 demo_server.py
    ```

    The server will start and listen for connections on all network interfaces.

2.  **Run the Client**:
    On a separate machine, run the client to connect to the server:

    ```bash
    python3 demo_cliente.py
    ```

    The client will try to automatically discover the server on the local network. If it fails, you can enter the server's IP address manually.

    Once connected, you'll see a menu with options to:
    - Read all sensors once.
    - Monitor the sensors continuously.
    - Change the server's IP address.

## Server Endpoints

The server exposes the following CoAP endpoints:

- `coap://<SERVER_IP>:5683/info`: Get information about the server.
- `coap://<SERVER_IP>:5683/sensors/temperature`: Get the current temperature reading.
- `coap://<SERVER_IP>:5683/sensors/humidity`: Get the current humidity reading.
- `coap://<SERVER_IP>:5683/sensors/pressure`: Get the current pressure reading.
- `coap://<SERVER_IP>:5683/sensors/all`: Get all sensor readings in a single request.

You can also use a command-line tool like `coap-client` (if installed) to interact with the server:

```bash
coap-client -m get coap://<SERVER_IP>/sensors/all
```
