/**
 * @file    modbus_tcp_tui.c
 * @author  Adrián Silva Palafox
 * @date    2024-06-09
 * @brief   Simple Modbus TCP client with a ncurses-based TUI for controlling a PLC.
 *
 *           Allows toggling RUN/STOP, FWD/REV, and setting frequency.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <ncurses.h>
#include <errno.h>
#include <modbus.h>
#include <sys/param.h>

// Uncomment to disable real Modbus communication for debugging UI only
// #define DEBUG_TUI

// PLC server parameters
#define SERVER_IP "192.168.0.52"

// ==== Global State Variables ====
// These represent the current state of the PLC as seen by the UI.
static bool run = false;          // false = STOP, true = RUN
static bool direction = false;    // false = FWD, true = REV
static int frequency = 0;         // Frequency in Hz * 100 (for Modbus scaling)

// ==== Function Prototypes ====
void push_button(modbus_t *mb, uint8_t reg);
void toggle_run_stop(modbus_t *mb);
void toggle_direction(modbus_t *mb);
void change_frequency(modbus_t *mb);
void draw_ui(void);

// ==== Modbus Button Simulation ====
// Simulates pressing a button by writing 1 then 0 to a Modbus register.
void push_button(modbus_t *mb, uint8_t reg) {
#ifndef DEBUG_TUI
  modbus_write_register(mb, reg, 1);
  modbus_write_register(mb, reg, 0);
#endif
}

// ==== Toggle RUN/STOP ====
// Switches between RUN and STOP states.
void toggle_run_stop(modbus_t *mb) {
  run = !run;
#ifndef DEBUG_TUI
  if (run) {
    push_button(mb, 2); // RUN button
  } else {
    push_button(mb, 3); // STOP button
  }
#endif
}

// ==== Toggle FWD/REV ====
// Switches between Forward and Reverse directions.
void toggle_direction(modbus_t *mb) {
  direction = !direction;
#ifndef DEBUG_TUI
  push_button(mb, 1); // FWD/REV button
#endif
}

// ==== Change Frequency ====
// Prompts the user for a new frequency and updates the PLC.
void change_frequency(modbus_t *mb) {
  char str_freq[5] = {0};
  int new_freq = 0;

  attron(A_BOLD);
  mvprintw(8, 2, "Enter new frequency (Hz): ");
  attroff(A_BOLD);
  echo();
  curs_set(1);
  getnstr(str_freq, 4);
  noecho();
  curs_set(0);

  if (sscanf(str_freq, "%d", &new_freq) == 1 && new_freq >= 0 && new_freq <= 60) {
    frequency = new_freq * 100; // Modbus expects frequency * 100
#ifndef DEBUG_TUI
    modbus_write_register(mb, 0, frequency);
#endif
  }
}

// ==== Draw Terminal UI ====
// Renders the TUI showing current state and controls.
void draw_ui(void) {
  clear();
  box(stdscr, 0, 0);

  // Title
  attron(A_BOLD | A_REVERSE);
  mvprintw(1, 2, "==== Modbus TCP Control Panel ====");
  attroff(A_BOLD | A_REVERSE);

  // Current state
  mvprintw(3, 2, "State     : %s", run ? "RUN" : "STOP");
  mvprintw(4, 2, "Direction : %s", direction ? "REV" : "FWD");
  mvprintw(5, 2, "Frequency : %d Hz", frequency / 100);

  // Menu options
  attron(A_BOLD);
  mvprintw(7, 2, "Controls:");
  attroff(A_BOLD);
  mvprintw(8, 4, "[1] RUN / STOP");
  mvprintw(9, 4, "[2] FWD / REV");
  mvprintw(10, 4, "[3] Change Frequency");
  mvprintw(11, 4, "[q] Quit");

  refresh();
}

// ==== Main Program Entry Point ====
int main(void) {
  modbus_t *mb = NULL;

#ifndef DEBUG_TUI
  // Initialize Modbus TCP connection
  mb = modbus_new_tcp(SERVER_IP, MODBUS_TCP_DEFAULT_PORT);
  if (mb == NULL) {
    fprintf(stderr, "Failed to allocate libmodbus context\n");
    return -1;
  }
  if (modbus_connect(mb) == -1) {
    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    modbus_free(mb);
    return -1;
  } else {
    printf("Connected to server successfully\n");
  }
#endif

  // Initialize ncurses
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);

  int ch;
  draw_ui();
  while ((ch = getch()) != 'q') {
    switch (ch) {
      case '1':
        toggle_run_stop(mb);
        break;
      case '2':
        toggle_direction(mb);
        break;
      case '3':
        change_frequency(mb);
        break;
      default:
        break;
    }
    draw_ui();
  }

  // Clean up and exit
  endwin();
#ifndef DEBUG_TUI
  modbus_close(mb);
  modbus_free(mb);
#endif
  printf("Application exited cleanly.\n");
  return 0;
}
