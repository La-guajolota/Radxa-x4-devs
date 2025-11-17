/**
 * @file vfd_driver.c
 * @brief Implementation of Modbus RTU communication logic.
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "vfd_driver.h"

int init_modbus_connection(modbus_config_t *conf) {
    conf->ctx = modbus_new_rtu(conf->device, conf->baud, conf->parity, 
                                conf->data_bit, conf->stop_bit);
    if (conf->ctx == NULL) {
        fprintf(stderr, "Unable to create libmodbus context\n");
        return -1;
    }

    modbus_set_slave(conf->ctx, conf->slave_id);
    
    // Debug off to prevent TUI corruption
    modbus_set_debug(conf->ctx, FALSE); 

    // Set response timeout to 500ms for responsive UI
    modbus_set_response_timeout(conf->ctx, 0, 500000);

    if (modbus_connect(conf->ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(conf->ctx);
        return -1;
    }

    return 0;
}

void update_telemetry(modbus_t *ctx, telemetry_t *tlm) {
    int rc = modbus_read_registers(ctx, REG_MONITOR_START, MONITOR_LEN, tlm->raw_buffer);
    
    if (rc == -1) {
        tlm->comm_error = true;
        snprintf(tlm->last_msg, 64, "ERR: Read Timeout/Fail");
        return;
    }
    
    tlm->comm_error = false;

    // Mapping based on 0x2103 block read
    // buffer[0]: Freq Command Monitor (xx.xx Hz)
    // buffer[1]: Output Current (x.xA approx)
    // buffer[3]: Output Voltage (x.xV approx)
    // buffer[9]: RPM
    tlm->freq_out    = tlm->raw_buffer[0] / 100.0; 
    tlm->current_amp = tlm->raw_buffer[1] / 10.0;  
    tlm->voltage_v   = tlm->raw_buffer[3] / 10.0;  
    tlm->rpm         = tlm->raw_buffer[9];         
}

void send_control_command(modbus_t *ctx, const setpoint_t *sp, telemetry_t *tlm) {
    // Construct Control Byte for MS300
    // Bits 0-1: 10 (2) Run, 01 (1) Stop
    uint16_t cmd_val = 0;
    
    if (sp->run_state) {
        cmd_val = 0b10; // Run bit
    } else {
        cmd_val = 0b01; // Stop bit
    }

    // Direction Bits
    // REV: bit 4 set to 1 (0x10)
    if (sp->direction) { // REV
        cmd_val |= (1 << 4); 
    } else { // FWD
        cmd_val |= (0 << 4); 
    }
    
    if (modbus_write_register(ctx, REG_CONTROL_WORD, cmd_val) == -1) {
        tlm->comm_error = true;
        snprintf(tlm->last_msg, 64, "ERR: Write CMD Fail");
    } else {
        tlm->comm_error = false;
    }
}

void send_freq_command(modbus_t *ctx, const setpoint_t *sp, telemetry_t *tlm) {
    if (modbus_write_register(ctx, REG_FREQ_CMD, sp->target_freq) == -1) {
        tlm->comm_error = true;
        snprintf(tlm->last_msg, 64, "ERR: Write Freq Fail");
    } else {
        tlm->comm_error = false;
        snprintf(tlm->last_msg, 64, "Set Freq: %.2f Hz", sp->target_freq / 100.0);
    }
}