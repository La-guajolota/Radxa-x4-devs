// Standard libs
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

// External libs
#include <modbus.h>
#include <modbus-rtu.h>

// Struct to hold VFD telemetry parameters and sensors  
typedef struct {
    // Command write only
    uint16_t freq_command;
    uint16_t mov_command;
    // Status monitor read
    uint16_t current_out;
    uint16_t voltage_out;
    uint16_t power_factor_angle_out;
    uint16_t rpm_out;
} vdf_t; 

// Struct to hold Modbus configuration
typedef struct {
    modbus_t *ctx;
    const char *device;
    int baud;
    char parity;
    int data_bit;
    int stop_bit;
    int slave_id;
} modbus_config_t;

// Function prototypes
int init_modbus_connection(modbus_config_t *conf);
void handle_shutdown(int signum);

// Global flag to control the main loop
volatile sig_atomic_t keep_running = 1;

int main() {
    modbus_config_t modbus_conf;
    vdf_t MS_300_controller;

    // Set up signal handlers for graceful shutdown
    signal(SIGINT, handle_shutdown);
    signal(SIGTERM, handle_shutdown);

    // Initialize Modbus connection
    modbus_conf.device = "/dev/ttyS4";
    modbus_conf.baud = 38400;
    modbus_conf.parity = 'N';
    modbus_conf.data_bit = 8;
    modbus_conf.stop_bit = 1;
    modbus_conf.slave_id = 2;

    // Assign Modbus-RTU registers to VFD  
    MS_300_controller.mov_command = 0x2000;
    MS_300_controller.freq_command = 0x2001;
    MS_300_controller.voltage_out = 0x2106;
    MS_300_controller.current_out = 0x2104;
    MS_300_controller.power_factor_angle_out = 0x210A;
    MS_300_controller.rpm_out = 0x210C;
    uint16_t vfd_registers[][2] = {
        {0x2001, 1}, // freq_command 
        {0x2104, 1}, // current_out
        {0x2106, 1}, // voltage_out
        {0x210A, 1}, // power_factor_angle_out
        {0x210C, 1}  // rpm_out
    };

    if (init_modbus_connection(&modbus_conf) != 0) {
        return EXIT_FAILURE;
    }

    printf("Modbus connection established. Starting main loop...\n");
    printf("Press Ctrl+C to exit.\n\n");

    uint16_t vfd_reg[10];

    while (keep_running) {
        if (modbus_read_registers(modbus_conf.ctx, 0x2103, 4, vfd_reg) == -1) {
            fprintf(stderr, "Modbus read error: %s\n", modbus_strerror(errno));
        } else {
            // Process the received data
            printf("Successfully read registers: ");
            for (int i = 0; i < 4; i++) {
                printf("0x%04X ", vfd_reg[i]);
            }
            printf("\n");
        }

        if (modbus_write_register(modbus_conf.ctx, 0x2001, 1500) == -1) {
            fprintf(stderr, "Modbus write error: %s\n", modbus_strerror(errno));
        } else {
            printf("Successfully wrote frequency command: 1500\n");
        }
    }

    printf("\nShutting down...\n");

    // Cleanup
    modbus_close(modbus_conf.ctx);
    modbus_free(modbus_conf.ctx);

    return EXIT_SUCCESS;
}

int init_modbus_connection(modbus_config_t *conf) {
    conf->ctx = modbus_new_rtu(conf->device, conf->baud, conf->parity, 
                                conf->data_bit, conf->stop_bit);
    if (conf->ctx == NULL) {
        fprintf(stderr, "Unable to create the libmodbus context\n");
        return -1;
    }

    modbus_set_slave(conf->ctx, conf->slave_id);
    modbus_set_debug(conf->ctx, TRUE);
        
    // Setting serial mode to RS485
    if (modbus_rtu_set_serial_mode(conf->ctx, MODBUS_RTU_RS485)) {
        printf("Serial mode: MODBUS_RTU_RS485\n");
    } else {
        fprintf(stderr, "Unable to set RS485 mode\n");
        modbus_free(conf->ctx);
        return -1;
    }

    // Set indication timeout
    if (modbus_set_indication_timeout(conf->ctx, 1, 0) == -1) {
        fprintf(stderr, "Couldn't set the indication timeout: %s\n", 
                modbus_strerror(errno));
        modbus_free(conf->ctx);
        return -1;
    }

    uint32_t indication_to_sec;
    uint32_t indication_to_usec;
    if (modbus_get_indication_timeout(conf->ctx, &indication_to_sec, 
                                      &indication_to_usec) == -1) {
        fprintf(stderr, "Couldn't get the indication timeout: %s\n", 
                modbus_strerror(errno));
        modbus_free(conf->ctx);
        return -1;
    }
    printf("Indication timeout: %d secs %d usecs\n", 
           indication_to_sec, indication_to_usec);
    
    // Set response timeout
    if (modbus_set_response_timeout(conf->ctx, 0, 200000) == -1) {
        fprintf(stderr, "Couldn't set the response timeout: %s\n", 
                modbus_strerror(errno));
        modbus_free(conf->ctx);
        return -1;
    }

    uint32_t response_to_sec;
    uint32_t response_to_usec;
    if (modbus_get_response_timeout(conf->ctx, &response_to_sec, 
                                    &response_to_usec) == -1) {
        fprintf(stderr, "Couldn't get the response timeout: %s\n", 
                modbus_strerror(errno));
        modbus_free(conf->ctx);
        return -1;
    }
    printf("Response timeout: %d secs %d usecs\n", 
           response_to_sec, response_to_usec);

    // Connect to Modbus
    if (modbus_connect(conf->ctx) == -1) {
        fprintf(stderr, "Connection to RS-485 bus failed: %s\n", 
                modbus_strerror(errno));
        modbus_free(conf->ctx);
        return -1;
    }

    printf("Successfully connected to Modbus RTU device\n");
    return 0;
}

void handle_shutdown(int signum) {
    printf("\nReceived signal %d, stopping...\n", signum);
    keep_running = 0;
}