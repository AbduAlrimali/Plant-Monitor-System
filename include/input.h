#ifndef SENSORS_H 
#define SENSORS_H

#include <Keypad.h>
#include <DHT.h>

// Sensor pin definitions
#define DHT_PIN 15
#define SOIL_PIN 34
#define GAS_PIN 35
#define LIGHT_PIN 39
#define TRIG_PIN 5
#define ECHO_PIN 18
#define RELAY_PIN 2
#define GREEN_PIN 16
#define RED_PIN 19
#define PUSHBUTTON_PIN 23
#define BUZZER_PIN 4

#define SENSORS_NUM 6

extern double sensorsData[];
extern String sensorsName[];
extern String unit[];

enum sensors { SENSOR_SOIL_MOISTURE=0, SENSOR_GAS, SENSOR_LIGHT, SENSOR_WATER, SENSOR_HUMIDITY, SENSOR_TEMPERATURE };

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
void manualIrrigation(void* pvParameters);
#endif
