#include <handler.h>
#include<hivemq.h>
#include "sensors.h"
#include <ArduinoJson.h>
#include <wifi_setup.h>

QueueHandle_t xEventQueue;

void handler_setup(){
    xTaskCreate(&sensor_reading, "ReadingSensors", 8192, NULL, 1, NULL);
    xTaskCreate(&eventHandlerTask, "EventHandler", 8192, NULL, 1, NULL); 
    xTaskCreate(&keeping_connection, "MQTTKeeper", 8192, NULL, 1, NULL);
    xTaskCreate(&wifiMonitoringTask, "WiFi Monitor", 4096, NULL, 1, NULL);
    xEventQueue = xQueueCreate(10, sizeof(SystemEvent_t));
    Serial.println("Handler is ready.");
}

static void sendData(){
    JsonDocument doc;
    doc["tmp"] = temperatureData;
    doc["distance"] = distanceData;
    doc["air"] = gasData;
    doc["light"] = lightData;
    doc["humidity"] = humidityData;
    doc["soil moisture"] = moistureData;

  //convert json to c string
  String output;
  serializeJson(doc, output);
  
  //send to hivemq
  send_hive(output.c_str());
}

static void activatePumb(){
    if(moistureData<80){
        Serial.println("Pumb Activated!");
    } else return;
    while(moistureData<80){
        digitalWrite(RELAY_PIN, HIGH);
    }
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Pumb Stopped!");
}

bool sendSystemEvent(SystemEvent_t event) {
    BaseType_t xStatus = xQueueSendToBack(xEventQueue, &event, (TickType_t) 10); 
    if (xStatus != pdPASS) {
        vTaskDelay(pdMS_TO_TICKS(500));
        return false;
    }
    vTaskDelay(pdMS_TO_TICKS(500));
    return true;
}

void eventHandlerTask(void* pvParameters){
  SystemEvent_t receivedEvent;
  while (1) {
        if (xQueueReceive(xEventQueue, &receivedEvent, portMAX_DELAY) == pdTRUE) {
            switch (receivedEvent) {
                case EVENT_ACTIVATE_PUMB:
                    activatePumb();
                    break;
                case EVENT_DATA_READY_FOR_HIVE_MQ:
                    sendData();
                    break;
                case EVENT_WIFI_CONNECTED:
                    Serial.println("WiFi connected.");
                    break;
                case EVENT_WIFI_DISCONNECTED:
                    Serial.println("WiFi disconnected.");
                    start_wifi();
                    break;
                default:
                    Serial.println("Unusual event occured!");
                    break;
            }
        }
    }
}