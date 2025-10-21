# <font color="#2196F3">🚀 Radxa Communications HAT</font>

This repository contains the hardware design files for a communications HAT for Radxa Single-Board Computers (SBCs). This HAT expands the capabilities of the Radxa SBC by providing multiple communication interfaces, making it suitable for a wide range of applications, including industrial automation, IoT, and robotics.

## <font color="#4CAF50">✨ Overview</font>

The Radxa Communications HAT is a versatile expansion board that provides CAN, RS485, and LoRa communication interfaces. It is designed to be a "Hardware Attached on Top" (HAT) for Radxa SBCs with a 40-pin GPIO header. The board is powered by an external 12V supply and includes onboard voltage regulators to provide the necessary 5V and 3.3V for the components.

## <font color="#FFC107">🌟 Features</font>

-   **CAN Interface:** 🚗 The HAT includes a CAN interface with a `TJA1051T-3` transceiver and an `MCP2515-xSO` CAN controller. The interface is accessible via a screw terminal.
-   **RS485 Interface:** 📡 An RS485 interface with a `MAX3485` transceiver is available on a screw terminal.
-   **LoRa Communication:** 📶 The board is equipped with an `SX1278` LoRa module, allowing for long-range wireless communication. An antenna connector is provided.
-   **Power Supply:** ⚡ The HAT is powered by a 12V external supply and has an onboard buck converter (`MP2307`) and linear regulators (`AMS1117-5.0` and `LD1117V33`) to provide stable 5V and 3.3V power to the HAT and the Radxa SBC.
-   **40-Pin GPIO Connector:** 🔌 The HAT connects to the Radxa SBC via a 40-pin GPIO connector.

## <font color="#00BCD4">💡 Hardware</font>

### Block Diagram

```
+-----------------+      +-----------------+      +-----------------+
|   12V Input     |----->|  Power Supply   |----->|  Radxa SBC      |
+-----------------+      | (5V & 3.3V)     |      +-----------------+
                         +-----------------+
                               |
                               |
+------------------------------+------------------------------+
|                              |                              |
|                              |                              |
v                              v                              v
+-----------------+      +-----------------+      +-----------------+
| CAN Interface   |      | RS485 Interface |      | LoRa Module     |
| (MCP2515, TJA1051) |      | (MAX3485)       |      | (SX1278)        |
+-----------------+      +-----------------+      +-----------------+
```

### Main Components

-   **CAN Controller:** Microchip MCP2515-xSO
-   **CAN Transceiver:** NXP TJA1051T-3
-   **RS485 Transceiver:** Maxim Integrated MAX3485
-   **LoRa Module:** Semtech SX1278
-   **Buck Converter:** Monolithic Power Systems MP2307
-   **5V Regulator:** Advanced Monolithic Systems AMS1117-5.0
-   **3.3V Regulator:** STMicroelectronics LD1117V33

### Connectors

-   **Power:** 2-pin screw terminal for 12V input.
-   **CAN:** 2-pin screw terminal.
-   **RS485:** 3-pin screw terminal.
-   **LoRa Antenna:** U.FL connector.
-   **Radxa SBC:** 40-pin GPIO header.

## <font color="#FF5722">📸 PCB Views</font>

| Front View | Back View |
| :--------: | :-------: |
| ![Front View](https://via.placeholder.com/400x300?text=Front+View+of+PCB) | ![Back View](https://via.placeholder.com/400x300?text=Back+View+of+PCB) |

*Please replace the placeholder images with actual pictures of the PCB.*

## <font color="#673AB7">⚙️ Fabrication</font>

The `production` directory contains the Gerber files, bill of materials (BOM), and other files required for manufacturing the PCB.

-   `Communication_HAT.zip`: Gerber files for PCB fabrication.
-   `bom.csv`: Bill of materials.
-   `positions.csv`: Component placement list (CPL) for automated assembly.
-   `netlist.ipc`: IPC-D-356 netlist file.

## <font color="#009688">🚀 How to Get Started</font>

The hardware design is located in the `HAT_communication_protocols/` directory.

1.  **Install KiCad:** If you don't have it already, download and install the latest version of KiCad from the official website: [https://www.kicad.org/](https://www.kicad.org/)
2.  **Open the Project:** Launch KiCad and open the `HAT_communication_protocols.kicad_pro` file.
3.  **Explore and Modify:** From the KiCad project manager, you can open the schematic (`.kicad_sch`) and PCB layout (`.kicad_pcb`) to view, analyze, and modify the design.

## <font color="#9C27B0">📜 License</font>

This project is licensed under the [MIT License](LICENSE).