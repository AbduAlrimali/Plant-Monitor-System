#ifndef SENSORS_H 
#define SENSORS_H

#include <Keypad.h>
#include <DHT.h>

// Sensor pin definitions
#define DHT_PIN 15
#define SOIL_PIN 34
#define GAS_PIN 32
#define LIGHT_PIN 35
#define TRIG_PIN 5
#define ECHO_PIN 18
#define RELAY_PIN 2
#define GREEN_PIN 16
#define RED_PIN 19

#define SENSORS_NUM 6

extern uint8_t rowPins[];
extern uint8_t colPins[];
extern double sensorsData[];
extern String sensorsName[];

enum sensors { SOIL_MOISTURE=0, GAS, LIGHT, WATER, HUMIDITY, TEMPERATURE };

// Function prototypes
float readTemperature();
float readHumidity();
int readSoilMoisture();
int readGas();
int readLightIntensity();
float readWater();
void sensor_reading(void* pvParameters);
void input_setup();
void handleKeypadEvent(void* pvParameters);
#endif
