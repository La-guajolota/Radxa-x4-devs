/**
 * @file main.c
 * @brief HTTP server for Modbus TCP control using Mongoose and libmodbus.
 *
 * This file implements a simple HTTP server that provides a REST API to control
 * and monitor a Modbus device (such as a VFD) over TCP. The server uses the
 * Mongoose library for HTTP handling and libmodbus for Modbus communication.
 * Thread safety is ensured using a mutex for Modbus operations.
 *
 * API Endpoints:
 *   - POST /api/run    : Toggle run/stop state of the device.
 *   - POST /api/dir    : Toggle forward/reverse direction.
 *   - POST /api/freq   : Set frequency (expects 'freq' parameter in body).
 *   - GET  /api/status : Get current frequency, run state, and direction.
 *
 * @author Adrián Silva Palafox
 * @date   October 2025
 */
// Standard libraries
#include <stdio.h>
#include <errno.h>
// External libraries
#include <modbus.h>

/* Master's parameters */
const char *device = "/dev/ttyS4";
const int baud = 115200;
const char parity = 'N';
const int data_bit = 8;
const int stop_bit = 1;

/* Slave's parameters */
const int REMOTE_ID = 10;
uint16_t tab_reg[32];

int main(void) {
    // Create a new RTU context
    modbus_t *rtu_ctx;
    rtu_ctx = modbus_new_rtu(device, baud, parity, data_bit, stop_bit);
    if (rtu_ctx == NULL) {
        fprintf(stderr, "Unable to create the libmodbus context\n");
        return -1;
    }

    // Try to connnect to the slave/server device
    if (modbus_connect(rtu_ctx) == -1){
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(rtu_ctx);
        return -1;
    }
    printf("Successfully connected to %s\n", device);

    modbus_set_slave(rtu_ctx, REMOTE_ID);
    // Read 2 registers from address 0 of server ID 10.
    modbus_read_registers(rtu_ctx, 0, 2, tab_reg);

    // Clean up
    modbus_close(rtu_ctx);
    modbus_free(rtu_ctx);
    return 0;
}