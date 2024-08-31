#ifndef SENSORS_H 
#define SENSORS_H

#include <Keypad.h>
#include <DHT.h>

// Sensor pin definitions
#define DHT_PIN 15
#define SOIL_PIN 32
#define GAS_PIN 34
#define LIGHT_PIN 35
#define TRIG_PIN 5
#define ECHO_PIN 18
#define RELAY_PIN 2

extern uint8_t rowPins[];
extern uint8_t colPins[];

extern int moistureData;
extern int gasData;
extern int lightData;
extern float waterData;
extern float humidityData;
extern float temperatureData;

// Function prototypes
int readTemperature();
float readHumidity();
int readSoilMoisture();
int readGas();
int readLightIntensity();
float readWater();
void sensor_reading(void* pvParameters);
void input_setup();
void handleKeypadEvent(void* pvParameters);
#endif
