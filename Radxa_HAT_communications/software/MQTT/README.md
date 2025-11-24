<!-- @format -->

# 🚀 Radxa HAT — MQTT Examples

![language](https://img.shields.io/badge/language-C-00599C?style=flat)&nbsp;![mqtt](https://img.shields.io/badge/MQTT-Orange?style=flat&logo=mosquitto&logoColor=white)&nbsp;![platform](https://img.shields.io/badge/platform-Linux-333?style=flat)

<p style="color:#0B6E4F">A small collection of MQTT examples and tools for the Radxa HAT family. This repository demonstrates how to build a simple MQTT publisher using the Eclipse Paho C client.</p>

---

**Quick Overview**

- **What:** Simple MQTT publisher example (synchronous Paho C client)
- **Where:** `MQTT_test_client/`
- **Build:** `make`
- **Run:** `./mqtt_pub`

---

**Quick Start**

1. Install dependencies (Ubuntu/Debian):

```bash
sudo apt update
sudo apt install -y libpaho-mqtt3c-dev build-essential
```

2. Build the publisher:

```bash
cd MQTT_test_client
make
```

3. Run the publisher (uses settings from source constants):

```bash
./mqtt_pub
```

Notes:

- The default broker address, topic and payload are defined in `MQTT_test_client/mqtt_pub.c` via `#define` constants. Edit those values to point to your broker (e.g. change `ADDRESS`, `TOPIC`, `CLIENTID`).
- `CLIENTID` should be unique per client connected to the broker.

---

**Files & Layout**

- `MQTT_test_client/Makefile` : build rules and helper targets (`all`, `clean`, `install-deps`).
- `MQTT_test_client/mqtt_pub` : built binary (created by `make`).
- `MQTT_test_client/mqtt_pub.c` : example publisher source code (Eclipse Paho).

**Build Targets** (from `Makefile`)

- `make` — builds `mqtt_pub`.
- `make clean` — removes build artifacts.
- `make install-deps` — installs `libpaho-mqtt3c-dev` and build tools on Debian/Ubuntu.

---

**Implementation Notes**

- The example uses synchronous Paho API: it connects, publishes a retained message with `QOS=1`, waits for confirmation, then disconnects.
- Default values in `mqtt_pub.c`:
  - `ADDRESS` — `tcp://192.168.1.110:1883`
  - `TOPIC` — `vdf/telemetry/current`
  - `CLIENTID` — `MiClienteRadxa_001`
  - `QOS` — `1` (at least once)
  - `retained` — `1` (message retained on broker)

If you want to publish to another broker without editing the source, you can recompile with different `-D` defines (careful with quoting), or simply edit the `#define` values and rebuild.

---

**Example Output**

When running `./mqtt_pub` you should see something like:

```
Connecting to broker tcp://192.168.1.110:1883...
Connected!
Publishing to topic: vdf/telemetry/current
Waiting for delivery confirmation...
Message delivered with token 1
```

---

**Troubleshooting**

- If connection fails, verify broker reachability and firewall rules.
- Ensure `libpaho-mqtt3c-dev` is installed and the correct `-lpaho-mqtt3c` library is available at link time.
- If you see delivery timeouts, check broker QoS support and network latency.

---

**Contributing**

Feel free to open issues or PRs to add subscriber examples, TLS support, or a dynamic CLI-based publisher/subscriber.

**License**

This repository does not currently include an explicit license. Add one if you plan to share or publish the code publicly.

---

Happy hacking! ⚡️  
`Radxa HAT — MQTT examples`

# ☁️ MQTT Communication

This directory is dedicated to examples and code for **MQTT (Message Queuing Telemetry Transport)** communication.

---

## 📖 About MQTT

> **MQTT** is a lightweight, publish-subscribe, machine-to-machine network protocol. It is designed for connections with remote locations where a "small code footprint" is required or the network bandwidth is limited. The publish-subscribe messaging pattern requires a message broker, making it ideal for IoT and distributed systems.

---

## 📂 Contents

This directory is currently a placeholder for future MQTT examples.

**Coming soon:**

- Connecting to an MQTT broker.
- Publishing and subscribing to topics.
- Integration with popular IoT platforms.

---
