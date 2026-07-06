/**
 * @file config.h
 * @brief Abstracted network configurations and MQTT routing maps.
 */

#ifndef CONFIG_H
#define CONFIG_H

// Network Identification Gateways (Masked placeholders)
const char *ssid     = "GENERIC_FIELD_NETWORK_SSID";       
const char *password = "UNRESOLVED_FIELD_PASSWORD_HASH";   

// Isolated Broker Infrastructure
const char *mqtt_broker   = "edge-gateway-node.secure-mesh.io";
const char *mqtt_username = "FIELD_NODE_ID_ALPHA";
const char *mqtt_password = "ENCRYPTED_NODE_KEY_STRING";
const int mqtt_port       = 8883; // Standard Encrypted TLS Port

// --- SYSTEM MQTT TOPICS VECTOR ---
const char *topic_command  = "node/control/actuator_command";  
const char *topic_feedback = "node/telemetry/device_status";  
const char *topic_flow     = "node/telemetry/sensor_stream"; 

#endif
