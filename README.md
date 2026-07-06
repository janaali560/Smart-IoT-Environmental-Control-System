
# Smart IoT Environmental Control System & Automated Gateway 🌐

**Author:** Jana Abdelrehim  
[cite_start]**Institution:** University of the West of England (UWE Bristol) — Mechatronics Engineering [cite: 49, 50]  

---

## 📌 1. Project Overview

[cite_start]This repository features the low-level embedded firmware and secure network architecture for an IoT-enabled closed-loop liquid regulation system powered by an **ESP32 microcontroller**[cite: 30, 31]. [cite_start]The system captures real-time data from fluid flow telemetry sensors, evaluates safety threshold compliance locally, executes automated motor overrides to isolate hardware faults, and publishes system statuses wirelessly via the **MQTT protocol**[cite: 14, 32, 33, 34].

> 🔒 *Note: To protect private network infrastructure and security signatures, all raw authentication keys, specific cloud broker URLs, and private WiFi credentials have been abstracted into generic variables. Secure, production-ready code snippets are detailed below.*

---

## 🛠️ 2. Hardware Tech Stack & Communication Architecture

### [cite_start]Hardware Components Interfaced [cite: 14, 15]
* [cite_start]**Microcontroller Unit:** ESP32 (Dual-core Xtensa) [cite: 30, 31]
* [cite_start]**Flow Sensors:** Pulse-output liquid flow sensor using hardware timer interrupts 
* [cite_start]**Actuation Layer:** DC Fluid Pump driven via an L298N H-Bridge Motor Driver (utilizing PWM for proportional velocity adjustment) [cite: 31]
* **Visual Diagnostics:** Dual-LED status indicators (Green: Normal operation; Red: Dynamic over-threshold fault state)

### [cite_start]Telemetry & Network Stack [cite: 14, 33, 34]
* **Wireless Protocol:** WiFi 802.11 b/g/n
* **Transport Layer Security (TLS):** Encrypted communication via port `8883` backed by a verified Root CA X.509 certificate validation layout
* **Messaging Protocol:** MQTT (Publish/Subscribe architecture)

---

## 📉 3. Automated Logic Flowchart

[cite_start]The ESP32 runs a continuous monitoring loop tracking system health and parsing network requests[cite: 31, 34]:

```mermaid
flowchart TD
    Start([System Boot]) --> Init[Initialize WiFi & Mount Secure Root CA]
    Init --> MQTT_Connect{MQTT Broker Connected?}
    
    MQTT_Connect -- No --> Retry[Attempt Broker Connection via TLS 8883] --> MQTT_Connect
    MQTT_Connect -- Yes --> Sub[Subscribe to Inbound Command Topic]
    
    Sub --> Run[Read Pulse Encoders via Hardware Interrupts]
    Run --> Calc[Compute Real-Time Flow Rate]
    Calc --> Pub[Publish Raw Flow Telemetry via MQTT]
    
    Pub --> CmdCheck{User 'Pump ON' State Active?}
    CmdCheck -- No --> StopPump[Isolate Actuator PWM to 0] --> LEDOff[LEDs Extinguished]
    CmdCheck -- Yes --> FlowCheck{Flow Rate <= Max Safe Limit?}
    
    FlowCheck -- Yes (Normal) --> RunPump[Drive Pump Motor via PWM] --> LEDGreen[Green Indicator HIGH]
    FlowCheck -- No (Fault) --> SafetyStop[Automated Safety Override: PWM 0] --> LEDRed[Red Indicator HIGH]
    
    LEDRed --> Alert[Publish Critical Error Status Packet]
    RunPump --> FeedbackLoop[Loop Routine Continues]
    StopPump --> FeedbackLoop

├── Firmware/
│   ├── config.h               # Network placeholders and topic configurations
│   └── secure_iot_gateway.ino # Main execution state-machine sequencer and callbacks
└── Documentation/

## ⚙️ Step 2: Create the Secure Code Snippets

Instead of a single large file, splitting your code into two modular files makes you look like a highly structured software engineer.

### File 1: Create `config.h`
Create a new file in your repository named `config.h`. This shows how you handle configuration parameters elegantly while masking your personal information:

```cpp
/**
 * @file config.h
 * @brief Network credentials and MQTT topic configurations.
 */

#ifndef CONFIG_H
#define CONFIG_H

// Network Identification Keys (Abstracted for Security)
const char *ssid     = "YOUR_SECURE_WIFI_SSID";       
const char *password = "YOUR_WIFI_WPA2_PASSWORD";   

// MQTT Server Infrastructure 
const char *mqtt_broker   = "your-secure-broker-subdomain.emqxsl.com";
const char *mqtt_username = "AUTHORIZED_GATEWAY_USER";
const char *mqtt_password = "SECURE_BROKER_PASSWORD";
const int mqtt_port       = 8883; // Encrypted MQTTS Port

// --- MQTT TOPICS ARCHITECTURE ---
const char *topic_command  = "emss/esp32/pump";    // Inbound subscriber control route
const char *topic_feedback = "emss/esp32/status";  // Outbound operational state transmission
const char *topic_flow     = "emss/esp32/flowrate"; // Outbound continuous telemetry matrix

#endif
    └── secure_broker_ca.pem   # Secure TLS certificate verification blueprint
