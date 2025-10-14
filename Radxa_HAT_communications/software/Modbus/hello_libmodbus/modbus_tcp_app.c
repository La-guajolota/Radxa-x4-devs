/**
 * @file   : modbus_tcp_app.c
 * @brief  : Simple Modbus TCP client application for PLC control.
 * @author : Adrián Silva Palafox
 * @date   : 2024-06-09
 * 
 * This application connects to a PLC server via Modbus TCP and allows the user
 * to control RUN/STOP state, direction (FWD/REV), and frequency via a text menu.
 * 
 * Author: [Your Name]
 * Date: [Date]
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include "modbus.h"

// PLC server parameters
#define SERVER_IP "192.168.0.52"

// ==== Global state variables ====
// 0 = STOP, 1 = RUN
bool run = false;
// 0 = FWD, 1 = REV
bool direction = false;
// Frequency in Hz (default 0)
int frequency = 0;

// ==== Simulated PLC button functions ====

/**
 * @brief Simulates a button press on the PLC by writing 1 then 0 to a register.
 * @param mb Pointer to the modbus context.
 * @param reg Register address to write.
 */
void push_btn(modbus_t *mb, uint8_t reg) {
  modbus_write_register(mb, reg, 1);
  modbus_write_register(mb, reg, 0);
}

/**
 * @brief Toggles the RUN/STOP state and sends the corresponding command to the PLC.
 * @param mb Pointer to the modbus context.
 */
void run_stop(modbus_t *mb) {
  run = !run; // Toggle state
  printf(">> State changed to: %s\n", run ? "RUN" : "STOP");
  if (run) {
    push_btn(mb, 2); // RUN command
  } else {
    push_btn(mb, 1); // STOP command
  }
}

/**
 * @brief Toggles the direction (FWD/REV) and sends the command to the PLC.
 * @param mb Pointer to the modbus context.
 */
void fwd_rev(modbus_t *mb) {
  direction = !direction; // Toggle direction
  printf(">> Direction changed to: %s\n", direction ? "REV" : "FWD");
  push_btn(mb, 3); // Direction command
}

/**
 * @brief Prompts the user to enter a new frequency and sends it to the PLC.
 * @param mb Pointer to the modbus context.
 */
void change_frequency(modbus_t* mb) {
  printf("Enter new frequency (Hz): ");
  scanf("%d", &frequency);
  printf(">> Frequency updated to: %d Hz\n", frequency);
  int freq_to_send = frequency * 10; // PLC expects value multiplied by 10
  modbus_write_register(mb, 0, freq_to_send);
}

// ==== Main program ====

/**
 * @brief Main entry point. Connects to the PLC and provides a text menu for control.
 */
int main() {
  // Initialize Modbus TCP communication
  modbus_t *mb = modbus_new_tcp(SERVER_IP, MODBUS_TCP_DEFAULT_PORT);
  if (mb == NULL) {
    fprintf(stderr, "Unable to allocate libmodbus context\n");
    return -1;
  }
  if (modbus_connect(mb) == -1) {
    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    modbus_free(mb);
    return -1;
  }

  int option;
  bool exit_program = false;

  while (!exit_program) {
    printf("\n===== MAIN MENU =====\n");
    printf("1. RUN / STOP (current: %s)\n", run ? "RUN" : "STOP");
    printf("2. FWD / REV (current: %s)\n", direction ? "REV" : "FWD");
    printf("3. Change frequency (current: %d Hz)\n", frequency);
    printf("4. Exit\n");
    printf("Select an option: ");
    scanf("%d", &option);

    switch (option) {
      case 1: run_stop(mb); break;
      case 2: fwd_rev(mb); break;
      case 3: change_frequency(mb); break;
      case 4:
        exit_program = true;
        printf("Exiting...\n");
        modbus_close(mb);
        modbus_free(mb);
        break;
      default:
        printf("Invalid option. Please try again.\n");
    }
  }

  return 0;
}
