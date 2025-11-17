/**
 * @file tui_display.c
 * @brief Implementation of Ncurses TUI logic.
 */

#include <ncurses.h>
#include <stdio.h>
#include "tui_display.h"
#include "vfd_driver.h" // Needed to call send commands on input

void init_tui(void) {
    initscr();
    cbreak();               // Disable line buffering
    noecho();               // Do not echo key presses
    curs_set(0);            // Hide cursor
    keypad(stdscr, TRUE);   // Enable special keys (arrows)
    nodelay(stdscr, TRUE);  // Non-blocking getch()
}

void cleanup_tui(void) {
    endwin(); // Restore terminal settings
}

void draw_ui(const setpoint_t *sp, const telemetry_t *tlm) {
    clear();
    box(stdscr, 0, 0);

    // Header
    attron(A_BOLD);
    mvprintw(1, 2, " DELTA MS300 VFD CONTROLLER (Modbus RTU) ");
    attroff(A_BOLD);
    
    mvprintw(1, 45, "Port: /dev/ttyS4 @ 38400"); // Note: Hardcoded in display for now

    // Section: Setpoints
    mvprintw(3, 2, "---- COMMANDS ----");
    mvprintw(4, 4, "Status Req : %s", sp->run_state ? "RUN" : "STOP");
    mvprintw(5, 4, "Dir Req    : %s", sp->direction ? "REV" : "FWD");
    mvprintw(6, 4, "Target Freq: %.2f Hz", sp->target_freq / 100.0);

    // Section: Telemetry
    mvprintw(3, 40, "---- TELEMETRY (0x21xx) ----");
    mvprintw(4, 42, "Output Freq: %.2f Hz", tlm->freq_out);
    mvprintw(5, 42, "Current    : %.1f A", tlm->current_amp);
    mvprintw(6, 42, "Voltage    : %.1f V", tlm->voltage_v);
    mvprintw(7, 42, "RPM        : %d", tlm->rpm);

    // Section: Status
    mvprintw(9, 2, "---- SYSTEM STATUS ----");
    if (tlm->comm_error) {
        attron(A_BLINK); // Use standard blink if colors not init
        mvprintw(10, 4, "COMMUNICATION ERROR!");
        attroff(A_BLINK);
    } else {
        mvprintw(10, 4, "Modbus Link: OK");
    }
    mvprintw(11, 4, "Log: %s", tlm->last_msg);

    // Section: Footer / Instructions
    attron(A_REVERSE);
    mvprintw(13, 2, " [1] Start/Stop | [2] Fwd/Rev | [ARROWS] Adjust Freq | [q] Quit ");
    attroff(A_REVERSE);

    refresh();
}

void process_input(modbus_t *ctx, setpoint_t *sp, telemetry_t *tlm, volatile int *keep_running) {
    int ch = getch();

    if (ch == ERR) return; // No key pressed

    bool cmd_changed = false;
    bool freq_changed = false;

    switch (ch) {
        case 'q':
        case 'Q':
            *keep_running = 0;
            break;
        case '1': // Toggle RUN/STOP
            sp->run_state = !sp->run_state;
            cmd_changed = true;
            snprintf(tlm->last_msg, 64, "CMD: %s", sp->run_state ? "START" : "STOP");
            break;
        case '2': // Toggle Direction
            sp->direction = !sp->direction;
            cmd_changed = true;
            snprintf(tlm->last_msg, 64, "CMD: Direction Changed");
            break;
        case KEY_UP: // Freq Up Coarse
            sp->target_freq += 100; // +1.00 Hz
            if (sp->target_freq > 6000) sp->target_freq = 6000;
            freq_changed = true;
            break;
        case KEY_DOWN: // Freq Down Coarse
            sp->target_freq -= 100; // -1.00 Hz
            if (sp->target_freq < 0) sp->target_freq = 0;
            freq_changed = true;
            break;
        case KEY_RIGHT: // Freq Up Fine
             sp->target_freq += 10; // +0.10 Hz
             if (sp->target_freq > 6000) sp->target_freq = 6000;
             freq_changed = true;
             break;
        case KEY_LEFT: // Freq Down Fine
             sp->target_freq -= 10; // -0.10 Hz
             if (sp->target_freq < 0) sp->target_freq = 0;
             freq_changed = true;
             break;
    }

    // Only write to Modbus if state changed (reduces traffic)
    if (cmd_changed) send_control_command(ctx, sp, tlm);
    if (freq_changed) send_freq_command(ctx, sp, tlm);
}