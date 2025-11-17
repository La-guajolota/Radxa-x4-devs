
/**
 * @file rtu-slave.c
 * @brief Modbus RTU Slave Simulator for Delta MS300 VFD.
 *
 * This application simulates a Delta MS300 VFD as a Modbus RTU slave device.
 * It responds to register reads/writes for control and telemetry, allowing integration
 * with master applications for testing and development.
 *
 * Features:
 *   - Slave ID: 2
 *   - Responds to writes at 0x2000 (Control) and 0x2001 (Frequency)
 *   - Responds to reads at 0x2103 (Telemetry block)
 *   - Simulates motor ramp-up/ramp-down and telemetry values
 *   - Uses libmodbus for RTU protocol handling
 *
 * Usage:
 *   $ ./rtu-slave [serial_port]
 *   Default port: /dev/ttyUSB1
 *
 * Dependencies:
 *   - libmodbus (https://libmodbus.org/)
 *
 * Compatibility:
 *   - Designed for Linux and tested with libmodbus >= 3.1.6
 *   - Register mapping matches Delta MS300 VFD for integration with TUI master
 *
 * @author Adrián Silva Palafox
 * @date November 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <modbus.h>
#include <time.h> 

// --- VFD Definitions ---
#define SLAVE_ID          2
#define BAUDRATE          38400
#define PARITY            'N'
#define DATA_BITS         8
#define STOP_BITS         1

// --- Register Definitions ---
#define REG_START_ADDR    0x2000
#define REG_BLOCK_SIZE    300     

// --- Offsets (Index 0 = REG_START_ADDR) ---
#define OFF_CONTROL_WORD  0x0000  // 0x2000
#define OFF_FREQ_CMD      0x0001  // 0x2001
#define OFF_MONITOR_START 0x0103  // 0x2103
#define OFF_MON_CURRENT   0x0104  // 0x2104
#define OFF_MON_VOLTAGE   0x0106  // 0x2106
#define OFF_MON_RPM       0x010C  // 0x210C

#define CMD_RUN           0x02

// --- Simulation State ---
typedef struct {
    double freq;
    double rpm;
    double amps;
    double volts;
} vfd_state_t;

// Global control flag for signal handler
volatile sig_atomic_t keep_running = 1;

/**
 * @brief Signal handler for graceful shutdown (SIGINT/SIGTERM).
 * @param sig Signal number.
 */
void handle_shutdown(int sig) {
    (void)sig;
    keep_running = 0;
}

/**
 * @brief Simulates one tick of VFD operation.
 * 
 * Reads control commands from Modbus mapping, updates VFD state,
 * and writes telemetry back to Modbus mapping.
 * 
 * @param mb_mapping Pointer to Modbus mapping structure.
 * @param vfd Pointer to VFD state structure.
 */
void run_simulation_tick(modbus_mapping_t *mb_mapping, vfd_state_t *vfd) {
    // Safety check to prevent segfaults if mapping failed
    if (!mb_mapping || !mb_mapping->tab_registers) return;

    // 1. Read commands
    uint16_t control_word = mb_mapping->tab_registers[OFF_CONTROL_WORD];
    uint16_t target_freq_raw = mb_mapping->tab_registers[OFF_FREQ_CMD];
    double target_freq = target_freq_raw / 100.0;

    int is_running = (control_word & CMD_RUN) == CMD_RUN;

    // 2. Physics logic (ramp up/down) 
    if (is_running) {
        if (vfd->freq < target_freq) {
            vfd->freq += 0.5; 
            if (vfd->freq > target_freq) vfd->freq = target_freq;
        } else if (vfd->freq > target_freq) {
            vfd->freq -= 0.5;
            if (vfd->freq < target_freq) vfd->freq = target_freq;
        }
    } else {
        if (vfd->freq > 0) {
            vfd->freq -= 1.0;
            if (vfd->freq < 0) vfd->freq = 0;
        }
    }

    // 3. Derived values (calculate RPM, Amps, Volts based on frequency)
    if (vfd->freq == 0) {
        vfd->rpm = 0; vfd->amps = 0; vfd->volts = 0;
    } else {
        vfd->rpm = vfd->freq * 30.0;
        vfd->amps = vfd->freq / 10.0;
        vfd->volts = vfd->freq * 3.8;
    }

    // 4. Write telemetry (scaled for Modbus registers)
    mb_mapping->tab_registers[OFF_MONITOR_START] = (uint16_t)(vfd->freq * 100);
    mb_mapping->tab_registers[OFF_MON_CURRENT] = (uint16_t)(vfd->amps * 10);
    mb_mapping->tab_registers[OFF_MON_VOLTAGE] = (uint16_t)(vfd->volts * 10);
    mb_mapping->tab_registers[OFF_MON_RPM] = (uint16_t)(vfd->rpm);
}

int main(int argc, char *argv[]) {
    modbus_t *ctx = NULL;
    modbus_mapping_t *mb_mapping = NULL;
    uint8_t query[MODBUS_RTU_MAX_ADU_LENGTH];
    int rc;

    const char *port = (argc > 1) ? argv[1] : "/dev/ttyUSB1";

    printf("Starting VFD Slave Simulator on %s...\n", port);

    ctx = modbus_new_rtu(port, BAUDRATE, PARITY, DATA_BITS, STOP_BITS);
    if (ctx == NULL) {
        fprintf(stderr, "Failed to create context: %s\n", modbus_strerror(errno));
        return -1;
    }

    // Timeout for non-blocking loop
    modbus_set_response_timeout(ctx, 0, 100000); // 100ms
    modbus_set_slave(ctx, SLAVE_ID);

    // Arguments: (Coils, NbCoils, InputBits, NbInputBits, Registers, NbRegisters, InputRegs, NbInputRegs)
    // We put REG_START_ADDR and REG_BLOCK_SIZE in the 5th and 6th arguments (Holding Registers)
    mb_mapping = modbus_mapping_new_start_address(
        0, 0, 
        0, 0, 
        REG_START_ADDR, REG_BLOCK_SIZE, 
        0, 0
    );
    
    if (mb_mapping == NULL) {
        fprintf(stderr, "Failed to allocate mapping: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_mapping_free(mb_mapping);
        modbus_free(ctx);
        return -1;
    }

    printf("Slave running (ID %d). Press Ctrl+C to stop.\n", SLAVE_ID);
    
    signal(SIGINT, handle_shutdown);
    signal(SIGTERM, handle_shutdown);

    // Optional: Enable debug output
    modbus_set_debug(ctx, TRUE); 

    vfd_state_t vfd_state = {0};

    while (keep_running) {
        rc = modbus_receive(ctx, query);
        
        if (rc > 0) {
            modbus_reply(ctx, query, rc, mb_mapping);
        } else if (rc == -1 && errno != ETIMEDOUT) {
            // Ignore errors during cleanup/disconnect to keep simulator alive if master drops
            if (errno != EBADF) { 
                 // Optional: print error only if verbose
            }
        }

        run_simulation_tick(mb_mapping, &vfd_state);
    }

    printf("\nShutting down slave...\n");
    modbus_mapping_free(mb_mapping);
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}