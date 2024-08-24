#ifndef VARIABLES_H 
#define VARIABLES_H

extern int soilData;
extern int _moisture;
extern int gasData;
extern int lightData;
extern float distanceData;
extern float humidity;
extern float temperature;

extern int soilPin;   // The ESP32 pin GPIO36 that reads the value from the water sensor
extern int airPin;
extern int lightPin;
extern int trigPin;    // Trigger
extern int echoPin;    // Echo
extern int DHTPin;
extern int relayPin;  // The ESP32 pin GPIO13 that connects to the relay to control the pump
extern int powerPin;  // The ESP32 pin GPIO32 that provides the power to the water sensor

void send_data();
void activate_pumb();

#endif