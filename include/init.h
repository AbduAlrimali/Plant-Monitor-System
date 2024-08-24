#ifndef VARIABLES_H 
#define VARIABLES_H

extern int soilData;
extern int _moisture;
extern int gasData;
extern int lightData;
extern float distanceData;
extern float humidity;
extern float temperature;

extern int soilPin=4;   // The ESP32 pin GPIO36 that reads the value from the water sensor
extern int airPin=34;
extern int lightPin=35;
extern int trigPin = 5;    // Trigger
extern int echoPin = 18;    // Echo
extern int DHTPin = 15;
extern int relayPin = 2;  // The ESP32 pin GPIO13 that connects to the relay to control the pump
extern int powerPin = 0;  // The ESP32 pin GPIO32 that provides the power to the water sensor

void send_data();
void activate_pumb();

typedef enum {
    EVENT_AIR_QUALITY_POOR=1,
    EVENT_DATA_READY_FOR_HIVE_MQ,
    EVENT_ACTIVATE_PUMB,
    EVENT_MANUAL_PUMB
} SystemEvent_t;

#endif