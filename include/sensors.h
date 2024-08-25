#ifndef SENSORS_H 
#define SENSORS_H

// Include necessary libraries
#include <DHT.h> 

// Sensor pin definitions
#define DHT_PIN 15
#define SOIL_PIN 4
#define GAS_PIN 34
#define LIGHT_PIN 35
#define TRIG_PIN 5
#define ECHO_PIN 18
#define RELAY_PIN 18

extern int moistureData;
extern int gasData;
extern int lightData;
extern float distanceData;
extern float humidityData;
extern float temperatureData;

// Function prototypes
float readTemperature();
float readHumidity();
int readSoilMoisture();
int readGas();
int readLightIntensity();
float readDistance();

#endif
