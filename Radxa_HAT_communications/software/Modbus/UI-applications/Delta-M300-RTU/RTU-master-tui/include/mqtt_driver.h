/**
 * @file mqtt_driver.h
 * @brief Interface for MQTT client operations and telemetry publishing.
 */

#ifndef MQTT_DRIVER_H
#define MQTT_DRIVER_H

#include "common.h"

/**
 * @brief Initializes the MQTT client and connects to the broker.
 * @param client Pointer to MQTTClient instance.
 * @param conn_opts Pointer to MQTTClient_connectOptions structure.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int init_mqtt_client(MQTTClient *client, MQTTClient_connectOptions *conn_opts);

/**
 * @brief Publishes telemetry data to the MQTT broker.
 * Formats telemetry as JSON and sends to the telemetry topic.
 * @param client Pointer to MQTTClient instance.
 * @param pubmsg Pointer to MQTTClient_message structure.
 * @param token Pointer to MQTTClient_deliveryToken for tracking delivery.
 * @param tlm Pointer to telemetry_t structure containing data to publish.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int publish_telemetry(MQTTClient *client, MQTTClient_message *pubmsg, MQTTClient_deliveryToken *token, telemetry_t *tlm);

/**
 * @brief Disconnects the MQTT client and cleans up resources.
 * @param client Pointer to MQTTClient instance.
 * @return int EXIT_SUCCESS always.
 */
int mqtt_disconnect(MQTTClient *client);

#endif // MQTT_DRIVER_H
