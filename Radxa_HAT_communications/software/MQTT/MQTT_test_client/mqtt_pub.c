/**
 * @file mqtt_pub.c
 * @brief Simple MQTT publish example using Eclipse Paho (Synchronous)
 *
 * This program:
 * 1. Connects to a public broker.
 * 2. Publishes a message to a topic.
 * 3. Disconnects.
 *
 * Compile with: gcc mqtt_pub.c -o mqtt_pub -lpaho-mqtt3c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h" // Header principal de Paho

// ==== Configuration ====
// We use a public broker for testing.
// Change this to your local broker IP if you have one (e.g. "tcp://localhost:1883")
#define ADDRESS "tcp://192.168.1.110:1883"
#define CLIENTID "MiClienteRadxa_001" // Must be unique in the broker
#define TOPIC "vdf/telemetry/current"
#define QOS 1
#define TIMEOUT 10000L

int main(int argc, char *argv[])
{
    // 1. Define client structures
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    // 2. Create the client instance (but does not connect to the network yet)
    // MQTTCLIENT_PERSISTENCE_NONE means if the program crashes, it does not save messages to disk.
    if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID,
                                MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
        printf("Error creating client, code: %d\n", rc);
        return EXIT_FAILURE;
    }

    // 3. Set connection options
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1; // 1 = Clean previous session when connecting

    // 4. Connect to the Broker
    printf("Connecting to broker %s...\n", ADDRESS);
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code: %d\n", rc);
        return EXIT_FAILURE;
    }
    printf("Connected!\n");

    // 5. Prepare the message (Payload)
    // Here you could send a JSON: "{\"freq\": 60.0, \"amp\": 5.5}"
    // char payload[] = "Hello from C on Linux!";
    char payload[] = "{\"current\": 60.0, \"voltage\": 230.0}";

    pubmsg.payload = payload;
    pubmsg.payloadlen = (int)strlen(payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 1; // 1 = Retain message in the broker

    // 6. Publish
    printf("Publishing to topic: %s\n", TOPIC);
    // This function is asynchronous for sending, it gives us a token to track it
    if ((rc = MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to start publishing, code: %d\n", rc);
    }

    // 7. Wait for confirmation (ACK) from the broker
    // We block the program until the message is delivered or TIMEOUT occurs
    printf("Waiting for delivery confirmation...\n");
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);

    if (rc == MQTTCLIENT_SUCCESS)
    {
        printf("Message delivered with token %d\n", token);
    }
    else
    {
        printf("Delivery error or timeout.\n");
    }

    // 8. Disconnect and clean up memory
    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
    {
        printf("Error disconnecting, code: %d\n", rc);
    }

    MQTTClient_destroy(&client);

    return EXIT_SUCCESS;
}