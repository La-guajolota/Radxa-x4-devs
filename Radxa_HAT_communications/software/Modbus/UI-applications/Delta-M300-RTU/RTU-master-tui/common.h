/**
 * @file common.h
 * @brief Shared definitions, constants, and data structures for the VFD Control Application.
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stdint.h>
#include <modbus.h>

// ==== Delta MS300 Register Definitions ====
#define REG_CONTROL_WORD  0x2000    ///< Control Word Register Address
#define REG_FREQ_CMD      0x2001    ///< Frequency Command Register Address
#define REG_MONITOR_START 0x2103    ///< Start Address for Monitor Registers
#define MONITOR_LEN       10        ///< Number of registers to read for telemetry

// ==== Binary Commands for Register 0x2000 ====
#define CMD_STOP          0x01      ///< Stop Command (0000 0001)
#define CMD_RUN           0x02      ///< Run Command (0000 0010)
#define CMD_FWD           0x00      ///< Forward Direction (Bits 4-5: 00)
#define CMD_REV           0x10      ///< Reverse Direction (Bit 4 ON)

/**
 * @brief Modbus Configuration Structure.
 */
typedef struct {
    modbus_t *ctx;      ///< Libmodbus context pointer
    const char *device; ///< Serial device path (e.g., /dev/ttyUSB0)
    int baud;           ///< Baud rate
    char parity;        ///< Parity ('N', 'E', 'O')
    int data_bit;       ///< Data bits (usually 8)
    int stop_bit;       ///< Stop bits (1 or 2)
    int slave_id;       ///< Modbus Slave ID
} modbus_config_t;

/**
 * @brief Desired State (Setpoints).
 * Stores the user's command intentions.
 */
typedef struct {
    bool run_state;     ///< true = RUN, false = STOP
    bool direction;     ///< false = FWD, true = REV
    int target_freq;    ///< Target frequency (Hz * 100)
} setpoint_t;

/**
 * @brief Received Telemetry (Feedback).
 * Stores data read from the VFD.
 */
typedef struct {
    uint16_t raw_buffer[MONITOR_LEN]; ///< Raw buffer for 0x2103 - 0x210C
    float current_amp;                ///< Output Current (Amps)
    float voltage_v;                  ///< Output Voltage (Volts)
    int rpm;                          ///< Motor Speed (RPM)
    float freq_out;                   ///< Output Frequency (Hz)
    bool comm_error;                  ///< Flag indicating Modbus communication failure
    char last_msg[64];                ///< Last status/error message for the UI
} telemetry_t;

#endif // COMMON_H