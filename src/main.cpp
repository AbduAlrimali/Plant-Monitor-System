#include <wifi_setup.h>
#include<hivemq.h>
#include <handler.h>
#include <sensors.h>

using namespace std;

void sensor_reading(void* pvParameters){
  while(1) {
    moistureData = readSoilMoisture();
    gasData = readGas();
    lightData = readLightIntensity();
    distanceData = readDistance();
    humidityData = readHumidity();
    temperatureData = readTemperature();

    sendSystemEvent(EVENT_DATA_READY_FOR_HIVE_MQ);
    if(moistureData < 20 || temperatureData>35) {
      sendSystemEvent(EVENT_ACTIVATE_PUMB);
    }
    vTaskDelay(15000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200); //UART

  pinMode(SOIL_PIN, INPUT);
  pinMode(GAS_PIN, INPUT);
  pinMode(LIGHT_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(DHT_PIN, INPUT);
  
  start_wifi();
  hivemq_setup();
  handler_setup();
}

void loop() {}