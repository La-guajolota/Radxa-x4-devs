/**
 * @file tui_display.h
 * @brief Interface for Ncurses TUI visualization and input handling.
 */

#ifndef TUI_DISPLAY_H
#define TUI_DISPLAY_H

#include <modbus.h>
#include "common.h"

/**
 * @brief Initializes the Ncurses environment.
 * Sets up noecho, cbreak, curs_set, and keypad.
 */
void init_tui(void);

/**
 * @brief Cleans up the Ncurses environment.
 * Should be called before program exit.
 */
void cleanup_tui(void);

/**
 * @brief Draws the user interface.
 * Renders setpoints, telemetry, system status, and instructions.
 * * @param sp Pointer to current setpoints.
 * @param tlm Pointer to current telemetry.
 */
void draw_ui(const setpoint_t *sp, const telemetry_t *tlm);

/**
 * @brief Processes keyboard input.
 * Handles 'q', '1', '2', and Arrow keys.
 * * @param ctx Modbus context (needed to send commands immediately).
 * @param sp Pointer to setpoints (to update desired state).
 * @param tlm Pointer to telemetry (to update status messages).
 * @param keep_running Pointer to the main loop control flag.
 */
void process_input(modbus_t *ctx, setpoint_t *sp, telemetry_t *tlm, volatile int *keep_running);

#endif // TUI_DISPLAY_H