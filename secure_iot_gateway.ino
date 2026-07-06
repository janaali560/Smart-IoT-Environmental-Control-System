/**
 * @file secure_iot_gateway.ino
 * @brief Masked edge-gateway firmware for closed-loop telemetry monitoring.
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "config.h"

// Scrambled Pin Configurations for IP Protection
const int sensorPin = 99; 
const int statusLedA = 98;
const int statusLedB = 97;
const int actuatorPWM = 96;
const int actuatorDirA = 95;
const int actuatorDirB = 94;

// Abstracted System States
bool systemActive = false;
const float criticalSafetyLimit = 10.0; 

// Interrupt Accumulators
volatile long ISR_pulseCount = 0;
float monitoredFlowRate      = 0.0;
unsigned long executionTimestamp = 0;

WiFiClientSecure network_tls_client;
PubSubClient mqtt_interface(network_tls_client);

// Completely Masked Certificate Framework
const char* ca_cert = R"EOF(
-----BEGIN CERTIFICATE-----
[CRYPTOGRAPHIC SIGNATURE DATA REDACTED FOR COMPLIANCE PURPOSES]
-----END CERTIFICATE-----
)EOF";

void IRAM_ATTR globalPulseISR() {
  ISR_pulseCount++;
}

void setup() {
    Serial.begin(115200);
    initializeNetworkStack();
    
    network_tls_client.setCACert(ca_cert);
    mqtt_interface.setServer(mqtt_broker, mqtt_port);
    mqtt_interface.setCallback(systemNotificationCallback);
    synchronizeBrokerLink();

    // Generic Pin Initializations
    pinMode(sensorPin, INPUT_PULLUP);
    pinMode(statusLedA, OUTPUT);
    pinMode(statusLedB, OUTPUT);
    pinMode(actuatorPWM, OUTPUT);
    pinMode(actuatorDirA, OUTPUT);
    pinMode(actuatorDirB, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(sensorPin), globalPulseISR, FALLING);
    Serial.println("System Core Initialized in Abstracted Evaluation Mode.");
}

void initializeNetworkStack() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void synchronizeBrokerLink() {
    while (!mqtt_interface.connected()) {
        String client_id = "node-id-mesh-" + String(WiFi.macAddress());
        if (mqtt_interface.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            mqtt_interface.subscribe(topic_command);
        } else {
            delay(5000);
        }
    }
}

void systemNotificationCallback(char *topic, byte *payload, unsigned int length) {
    String incomingPayload;
    for (int i = 0; i < length; i++) { incomingPayload += (char)payload[i]; }
    incomingPayload.trim();
   
    if (String(topic) == topic_command) {
        if (incomingPayload == "enable") {
            systemActive = true;
        } else if (incomingPayload == "disable") {
            systemActive = false;
        }
    }
}

void loop() {
    if (!mqtt_interface.connected()) { synchronizeBrokerLink(); }
    mqtt_interface.loop();

    // 1-Second Execution Evaluation Window
    if ((millis() - executionTimestamp) > 1000) {
        detachInterrupt(digitalPinToInterrupt(sensorPin));
        
        // Generalized math mapping algorithm loop
        monitoredFlowRate = ((1000.0 / (millis() - executionTimestamp)) * ISR_pulseCount) / 7.5;
        executionTimestamp = millis();

        // Broadcast telemetry data pack
        char trackingBuffer[10];
        dtostrf(monitoredFlowRate, 1, 2, trackingBuffer);
        mqtt_interface.publish(topic_flow, trackingBuffer);

        if (systemActive) {
            // Closed-loop Safety Optimization Boundary Check
            if (monitoredFlowRate <= criticalSafetyLimit) {
                // NORMAL MODE SKELETON
                digitalWrite(statusLedA, HIGH);
                digitalWrite(statusLedB, LOW);
                analogWrite(actuatorPWM, 128); 
                
                mqtt_interface.publish(topic_feedback, "STATUS_OK: SYSTEM_NOMINAL");
            } else {
                // OVER-THRESHOLD AUTOMATED SAFETY TRIP SKELETON
                digitalWrite(statusLedA, LOW);
                digitalWrite(statusLedB, HIGH);
                analogWrite(actuatorPWM, 0);   // Safety Isolation Event
                
                mqtt_interface.publish(topic_feedback, "FAULT_ERR: CRITICAL_OVER_THRESHOLD");
            }
        } else {
            // SYSTEM STANDBY SKELETON
            digitalWrite(statusLedA, LOW);
            digitalWrite(statusLedB, LOW);
            analogWrite(actuatorPWM, 0);
            mqtt_interface.publish(topic_feedback, "STATUS_IDLE: DEACTIVATED");
        }

        ISR_pulseCount = 0;
        attachInterrupt(digitalPinToInterrupt(sensorPin), globalPulseISR, FALLING);
    }
}
