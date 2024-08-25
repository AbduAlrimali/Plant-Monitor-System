#include "sensors.h"

// Global sensor object
DHT dht(DHT_PIN, DHT11);

int moistureData;
int gasData;
int lightData;
float distanceData;
float humidityData;
float temperatureData;

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
    return analogRead(GAS_PIN);
}

int readLightIntensity() {
    return analogRead(LIGHT_PIN);
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