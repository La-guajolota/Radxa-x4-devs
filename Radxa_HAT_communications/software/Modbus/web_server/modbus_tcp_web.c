/**
 * @file modbus_tcp_web.c
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
 * @date   September 2025
 */

#include "mongoose.h"
#include <modbus.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>

// Uncomment to disable real Modbus communication for debugging
// #define DEBUG_WEB

// PLC server parameters
#define SERVER_IP "192.168.0.52"

static modbus_t *mb; /**< Global Modbus context pointer */
static pthread_mutex_t mb_lock = PTHREAD_MUTEX_INITIALIZER; /**< Mutex for Modbus operations */

// ==== Global State Variables ====
// These represent the current state of the PLC as seen by the web interface.
static bool run = false;          // false = STOP, true = RUN
static bool direction = false;    // false = FWD, true = REV
static int frequency = 0;         // Frequency in Hz * 100 (for Modbus scaling)

// ==== Modbus Button Simulation ====
// Simulates pressing a button by writing 1 then 0 to a Modbus register.
void push_button(modbus_t *mb, uint8_t reg) {
#ifndef DEBUG_WEB
    pthread_mutex_lock(&mb_lock);
    modbus_write_register(mb, reg, 1);
    modbus_write_register(mb, reg, 0);
    pthread_mutex_unlock(&mb_lock);
#else
    (void)mb;   // Suppress unused parameter warning
    (void)reg;  // Suppress unused parameter warning
#endif
}

/**
 * @brief Toggles the run/stop state of the Modbus device.
 * @param mb Pointer to the Modbus context.
 */
void run_stop(modbus_t *mb) {
    run = !run;
#ifndef DEBUG_WEB
    if (run) {
        push_button(mb, 2); // RUN button
    } else {
        push_button(mb, 3); // STOP button
    }
#else
    (void)mb;  // Suppress unused parameter warning
#endif
    printf("Estado cambiado a: %s\n", run ? "RUN" : "STOP");
}

/**
 * @brief Toggles the forward/reverse direction of the Modbus device.
 * @param mb Pointer to the Modbus context.
 */
void fwd_rev(modbus_t *mb) {
    direction = !direction;
#ifndef DEBUG_WEB
    push_button(mb, 1); // FWD/REV button
#else
    (void)mb;  // Suppress unused parameter warning
#endif
    printf("Dirección cambiada a: %s\n", direction ? "REV" : "FWD");
}

/**
 * @brief Changes the frequency of the Modbus device.
 * @param mb Pointer to the Modbus context.
 * @param freq Frequency value to set.
 */
void cambiar_frecuencia(modbus_t *mb, int freq) {
    if (freq >= 0 && freq <= 60) {
        frequency = freq * 100; // Modbus expects frequency * 100
#ifndef DEBUG_WEB
        pthread_mutex_lock(&mb_lock);
        modbus_write_register(mb, 0, frequency);
        pthread_mutex_unlock(&mb_lock);
#else
        (void)mb;  // Suppress unused parameter warning
#endif
        printf("Frecuencia cambiada a: %d Hz\n", freq);
    }
}

/**
 * @brief Retrieves the status of the Modbus device.
 * @param mb Pointer to the Modbus context.
 * @param freq Pointer to store the current frequency.
 * @param run_state Pointer to store the run state.
 * @param dir Pointer to store the direction.
 * @return 0 on success, non-zero on failure.
 */
int get_status(modbus_t *mb, int *freq, int *run_state, int *dir) {
    // Return current state (in a real implementation, you might read from Modbus)
    *freq = frequency / 100;  // Convert back from Modbus scaling
    *run_state = run ? 1 : 0;
    *dir = direction ? 1 : 0; // 0 = FWD, 1 = REV
    
#ifdef DEBUG_WEB
    (void)mb;  // Suppress unused parameter warning
#endif
    
    return 0;
}

/**
 * @brief HTTP handler for /api/run endpoint.
 */
static void handle_run(struct mg_connection *c) {
    run_stop(mb);
    mg_http_reply(c, 200, "Content-Type: application/json\r\n", 
                 "{\"status\":\"ok\",\"action\":\"run_toggle\",\"state\":\"%s\"}", 
                 run ? "RUN" : "STOP");
}

/**
 * @brief HTTP handler for /api/dir endpoint.
 */
static void handle_dir(struct mg_connection *c) {
    fwd_rev(mb);
    mg_http_reply(c, 200, "Content-Type: application/json\r\n", 
                 "{\"status\":\"ok\",\"action\":\"dir_toggle\",\"direction\":\"%s\"}", 
                 direction ? "REV" : "FWD");
}

/**
 * @brief HTTP handler for /api/freq endpoint.
 */
static void handle_freq(struct mg_connection *c, struct mg_http_message *hm) {
    char freq_str[32];
    
    if (mg_http_get_var(&hm->body, "freq", freq_str, sizeof(freq_str)) > 0) {
        int freq = atoi(freq_str);
        if (freq >= 0 && freq <= 60) {
            cambiar_frecuencia(mb, freq);
            mg_http_reply(c, 200, "Content-Type: application/json\r\n", 
                         "{\"status\":\"ok\",\"action\":\"freq_set\",\"frequency\":%d}", freq);
        } else {
            mg_http_reply(c, 400, "Content-Type: application/json\r\n", 
                         "{\"status\":\"error\",\"message\":\"Invalid frequency range (0-60 Hz)\"}");
        }
    } else {
        mg_http_reply(c, 400, "Content-Type: application/json\r\n", 
                     "{\"status\":\"error\",\"message\":\"Missing freq parameter\"}");
    }
}

/**
 * @brief HTTP handler for /api/status endpoint.
 */
static void handle_status(struct mg_connection *c) {
    int freq, run_state, dir;
    
    if (get_status(mb, &freq, &run_state, &dir) == 0) {
        mg_http_reply(c, 200, "Content-Type: application/json\r\n", 
                     "{\"frequency\":%d,\"running\":%s,\"direction\":\"%s\",\"state\":\"%s\"}", 
                     freq, 
                     run_state ? "true" : "false", 
                     dir ? "REV" : "FWD",
                     run_state ? "RUN" : "STOP");
    } else {
        mg_http_reply(c, 500, "Content-Type: application/json\r\n", 
                     "{\"status\":\"error\",\"message\":\"Failed to read device status\"}");
    }
}

/**
 * @brief Mongoose event handler and HTTP dispatcher.
 */
static void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;

        if (mg_match(hm->uri, mg_str("/api/run"), NULL)) {
            handle_run(c);
        } else if (mg_match(hm->uri, mg_str("/api/dir"), NULL)) {
            handle_dir(c);
        } else if (mg_match(hm->uri, mg_str("/api/freq"), NULL)) {
            handle_freq(c, hm);
        } else if (mg_match(hm->uri, mg_str("/api/status"), NULL)) {
            handle_status(c);
        } else {
            struct mg_http_serve_opts opts = {.root_dir = "www"};
            mg_http_serve_dir(c, hm, &opts);
        }
    }
}

/**
 * @brief Main entry point. Initializes Modbus and HTTP server.
 */
int main(void) {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);

#ifndef DEBUG_WEB
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
        printf("Connected to Modbus server %s successfully\n", SERVER_IP);
    }
#else
    printf("Running in DEBUG mode - no real Modbus communication\n");
#endif

    // Levantar servidor HTTP en puerto 8000
    if (mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, NULL) == NULL) {
        fprintf(stderr, "Error iniciando servidor web\n");
#ifndef DEBUG_WEB
        modbus_close(mb);
        modbus_free(mb);
#endif
        return 1;
    }

    printf("Servidor web corriendo en http://localhost:8000\n");
    printf("Presiona Ctrl+C para salir\n");

    // Event loop
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }

    // Cleanup (nunca se alcanza con el loop infinito, pero está para completitud)
#ifndef DEBUG_WEB
    modbus_close(mb);
    modbus_free(mb);
#endif
    mg_mgr_free(&mgr);
    return 0;
}