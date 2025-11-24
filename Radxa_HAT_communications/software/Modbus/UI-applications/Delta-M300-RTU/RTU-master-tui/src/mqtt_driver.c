/**
 * @file mqtt_driver.c
 * @brief Implementation of MQTT client operations and telemetry publishing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mqtt_driver.h"

/**
 * @brief Initializes the MQTT client and connects to the broker.
 * @param client Pointer to MQTTClient instance.
 * @param conn_opts Pointer to MQTTClient_connectOptions structure.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int init_mqtt_client(MQTTClient *client, MQTTClient_connectOptions *conn_opts){
    int rc;

    // Create the client instance
    if ((rc = MQTTClient_create(client, ADDRESS, CLIENTID,
                                MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
        printf("Error creating client, code: %d\n", rc);
        return EXIT_FAILURE;
    }

    // Set connection options
    conn_opts->keepAliveInterval = 20;
    conn_opts->cleansession = 1;
    conn_opts->reliable = 1;
    conn_opts->connectTimeout = 10;

    // Connect to the Broker (client is already MQTTClient*, dereference it)
    if ((rc = MQTTClient_connect(*client, conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect MQTT broker, return code: %d\n", rc);
        printf("Check if broker at %s is running and reachable\n", ADDRESS);
        return EXIT_FAILURE;
    }

    printf("Successfully connected to MQTT broker\n");
    return EXIT_SUCCESS;
}

/**
 * @brief Publishes telemetry data to the MQTT broker.
 * Formats telemetry as JSON and sends to the telemetry topic.
 * @param client Pointer to MQTTClient instance.
 * @param pubmsg Pointer to MQTTClient_message structure.
 * @param token Pointer to MQTTClient_deliveryToken for tracking delivery.
 * @param tlm Pointer to telemetry_t structure containing data to publish.
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int publish_telemetry(MQTTClient *client, MQTTClient_message *pubmsg, MQTTClient_deliveryToken *token, telemetry_t *tlm) {
    int rc;
    
    // Prepare the message (Payload) - use static to persist beyond function scope
    static char payload0[254];
    snprintf(payload0, sizeof(payload0),
             "{\"current_amp\": %.2f, \"voltage_v\": %.2f, \"rpm\": %d, \"freq_out\": %.2f, \"comm_error\": %d, \"last_msg_code\": %d}",
             tlm->current_amp,
             tlm->voltage_v,
             tlm->rpm,
             tlm->freq_out,
             tlm->comm_error ? 1 : 0,
             tlm->last_msg_code);

    pubmsg->payload = payload0;
    pubmsg->payloadlen = (int)strlen(payload0);
    pubmsg->qos = QOS;
    pubmsg->retained = 0;

    // Publish (dereference client pointer)
    if ((rc = MQTTClient_publishMessage(*client, TOPIC_TELEMETRY, pubmsg, token)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to start publishing, code: %d\n", rc);
        return EXIT_FAILURE;
    }
    
    // Wait for confirmation from broker
    rc = MQTTClient_waitForCompletion(*client, *token, TIMEOUT);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to complete publishing, code: %d\n", rc);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
 * @brief Disconnects the MQTT client and cleans up resources.
 * @param client Pointer to MQTTClient instance.
 * @return int EXIT_SUCCESS always.
 */
int mqtt_disconnect(MQTTClient *client){
    int rc;

    // Disconnect (dereference client pointer)
    if ((rc = MQTTClient_disconnect(*client, 10000)) != MQTTCLIENT_SUCCESS)
    {
        printf("Error disconnecting, code: %d\n", rc);
    }

    // Destroy takes pointer to handle
    MQTTClient_destroy(client);

    return EXIT_SUCCESS;
}
