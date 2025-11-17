/**
 * @file main.c
 * @brief Main entry point for the VFD Control TUI.
 * Orchestrates initialization, the main loop, signal handling, and cleanup. 
 * 
 * @author Adrián Silva Palafox
 * @date October 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <ncurses.h>

#include "common.h"
#include "vfd_driver.h"
#include "tui_display.h"

// Global control flag for signal handler
volatile sig_atomic_t keep_running = 1;

/**
 * @brief Signal handler for graceful shutdown (SIGINT/SIGTERM).
 * @param signum Signal number.
 */
void handle_shutdown(int signum) {
    (void)signum; // Unused parameter
    keep_running = 0;
}

int main() {
    modbus_config_t modbus_conf;
    
    // State instances
    setpoint_t sp = { .run_state = false, .direction = false, .target_freq = 0 };
    telemetry_t tlm = {0};

    // Register Signals
    signal(SIGINT, handle_shutdown);
    signal(SIGTERM, handle_shutdown);

    // Modbus Configuration
    modbus_conf.device = "/dev/ttyS4";
    modbus_conf.baud = 38400;
    modbus_conf.parity = 'N';
    modbus_conf.data_bit = 8;
    modbus_conf.stop_bit = 1;
    modbus_conf.slave_id = 2;

    // Initialize Modbus
    if (init_modbus_connection(&modbus_conf) != 0) {
        return EXIT_FAILURE;
    }

    // Initialize UI
    init_tui();

    uint8_t cycle_count = 0;

    // Main Loop
    while (keep_running) {
        // 1. Process User Input
        // Note: Cast keep_running to non-atomic int pointer or handle inside carefully.
        // Here we pass the address of the volatile variable.
        process_input(modbus_conf.ctx, &sp, &tlm, (int *)&keep_running);

        // 2. Read Telemetry (approx every 200ms)
        if (cycle_count++ > 10) {
            update_telemetry(modbus_conf.ctx, &tlm);
            cycle_count = 0;
        }

        // 3. Draw Interface
        draw_ui(&sp, &tlm);

        // 4. Sleep to save CPU
        napms(20); // 20ms
    }

    // Cleanup UI
    cleanup_tui();
    
    // Safety: Stop motor on exit
    modbus_write_register(modbus_conf.ctx, REG_CONTROL_WORD, CMD_STOP);
    
    // Cleanup Modbus
    modbus_close(modbus_conf.ctx);
    modbus_free(modbus_conf.ctx);
    
    printf("Shutdown complete.\n");

    return EXIT_SUCCESS;
}