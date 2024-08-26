#include "sensors.h"
#include <DHT.h> 

int moistureData;
int gasData;
int lightData;
float distanceData;
float humidityData;
float temperatureData;

DHT dht(DHT_PIN, DHT11);

float readTemperature() {
    return dht.readTemperature(); 
}

float readHumidity() {
    return dht.readHumidity();
}

int readSoilMoisture() {
    int soilData = analogRead(SOIL_PIN);
    return 100 - ((soilData / 4095.00) * 100); 
}

int readGas() {
    int gasData = analogRead(GAS_PIN);
    return 100 - ((gasData / 4095.00) * 100); 
}

int readLightIntensity() {
    int lightData = analogRead(LIGHT_PIN);
    return 100 - ((lightData / 4095.00) * 100); 
}

float readDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    int duration = pulseIn(ECHO_PIN, HIGH);
    return (duration * 0.034) / 2;
}