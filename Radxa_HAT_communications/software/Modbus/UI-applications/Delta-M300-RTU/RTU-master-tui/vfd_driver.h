/**
 * @file vfd_driver.h
 * @brief Interface for Modbus RTU communication with Delta MS300 VFD.
 */

#ifndef VFD_DRIVER_H
#define VFD_DRIVER_H

#include "common.h"

/**
 * @brief Initializes the Modbus RTU connection.
 * * @param conf Pointer to the configuration structure.
 * @return int 0 on success, -1 on failure.
 */
int init_modbus_connection(modbus_config_t *conf);

/**
 * @brief Reads telemetry data from the VFD.
 * * Reads a block of registers starting at REG_MONITOR_START.
 * Updates the telemetry_t structure with parsed values.
 * * @param ctx Modbus context.
 * @param tlm Pointer to the telemetry structure to update.
 */
void update_telemetry(modbus_t *ctx, telemetry_t *tlm);

/**
 * @brief Sends the control word (Run/Stop/Direction) to the VFD.
 * * Constructs the bitmask based on setpoint state and writes to REG_CONTROL_WORD.
 * * @param ctx Modbus context.
 * @param sp Pointer to current setpoints.
 * @param tlm Pointer to telemetry (to update error/status messages).
 */
void send_control_command(modbus_t *ctx, const setpoint_t *sp, telemetry_t *tlm);

/**
 * @brief Sends the frequency command to the VFD.
 * * Writes the target frequency to REG_FREQ_CMD.
 * * @param ctx Modbus context.
 * @param sp Pointer to current setpoints.
 * @param tlm Pointer to telemetry (to update error/status messages).
 */
void send_freq_command(modbus_t *ctx, const setpoint_t *sp, telemetry_t *tlm);

#endif // VFD_DRIVER_H