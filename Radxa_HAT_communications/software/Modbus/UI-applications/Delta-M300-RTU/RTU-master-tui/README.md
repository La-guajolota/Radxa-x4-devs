## ✨ Features

- ✅ Control the VFD: Start / Stop, Forward / Reverse
- 🎚️ Adjust target frequency (coarse and fine steps)
- 📊 Read and display telemetry: frequency, current, voltage, RPM
- 📡 Publish telemetry over MQTT for remote monitoring

## 📁 Repository layout (current)

| Folder | Purpose |
|---|---|
| `src/` | C source files used by the build (`main.c`, `vfd_driver.c`, `tui_display.c`, `mqtt_driver.c`) |
| `include/` | Public headers (`common.h`, `vfd_driver.h`, `tui_display.h`, `mqtt_driver.h`) |
| `build/` | Object files (generated) |
| `bin/` | Binary output after building |
| `.vscode/`, `.clangd` | Editor and clangd configuration |
| `Makefile` | Build rules and helper targets |

## 🚀 Quick start (dependencies, build, run)

1. Install required system packages (Debian/Ubuntu):

```bash
make install-deps
```

2. Build and run:

```bash
make clean
make
./bin/delta_m300_vfd_rtu_tui
```

> Note: the program opens `/dev/ttyS4` by default. Either run with permissions to access that device or change the device path in `src/main.c` or `include/common.h`.

3. Remove build artifacts:

```bash
make clean
```

## 🧰 Editor & tooling

- VS Code: project's `.vscode/settings.json` is configured to include `${workspaceFolder}/include` and `${workspaceFolder}/src` for IntelliSense.
- clangd: `.clangd` contains project include flags and `-std=gnu99`. For exact compile flags consider generating `compile_commands.json` with `bear` or by switching to CMake:

```bash
sudo apt install bear
bear -- make clean && bear -- make
```

This produces `compile_commands.json` which clangd, language servers and tools can consume.

## 📚 Files and (concise) explanations

Below are the main headers and sources and how they fit together.

### `include/common.h`
- Shared types and constants: Modbus register definitions, command masks, MQTT topic/config, and data structures (`modbus_config_t`, `setpoint_t`, `telemetry_t`).

### `include/vfd_driver.h` + `src/vfd_driver.c`
- Modbus RTU wrapper using `libmodbus`:
  - `init_modbus_connection()` — create and configure RTU context and connect.
  - `update_telemetry()` — read telemetry registers and parse to engineering units.
  - `send_control_command()` / `send_freq_command()` — write control/frequency registers.

### `include/tui_display.h` + `src/tui_display.c`
- ncurses UI layer:
  - `init_tui()` / `cleanup_tui()` — terminal setup/restore.
  - `draw_ui()` — renders setpoints, telemetry, and status.
  - `process_input()` — non-blocking input handling and triggers commands.

### `include/mqtt_driver.h` + `src/mqtt_driver.c`
- MQTT integration (Paho C client):
  - `init_mqtt_client()` — create & connect to the broker.
  - `publish_telemetry()` — format telemetry into JSON and publish.
  - `mqtt_disconnect()` — graceful shutdown of the client.

### `src/main.c`
- Orchestrates initialization, main loop (input → telemetry read → publish → UI refresh), signal handling and cleanup.

## 🛠️ Notes & suggestions

- Configuration: serial device and MQTT address are currently hardcoded. I recommend adding CLI args (`--device`, `--baud`, `--broker`) or a config file.
- Robustness: add retries/backoff for libmodbus and MQTT reconnection logic.
- JSON safety: escape `last_msg` before embedding in JSON payloads (to avoid breaking quotes/newlines).
- Testing: add a mock mode to simulate Modbus responses so CI can run without hardware.

## ✅ Next steps I can take for you

- ✂️ Remove or archive top-level duplicate `.c`/`.h` files left from reorganization.
- ⚙️ Add a `Makefile` target to produce `compile_commands.json` using `bear`.
- 🔧 Convert the build to CMake for easier IDE integration and automatic `compile_commands.json` generation.
- 🧾 Add CLI configuration parsing and a runtime configuration file.

If you want any of the above, tell me which and I'll implement it.

---
*README refreshed with improved layout, emojis and clearer developer notes.*