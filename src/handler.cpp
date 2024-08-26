#include <handler.h>
#include<hivemq.h>
#include "sensors.h"
#include <wifi_setup.h>

QueueHandle_t xEventQueue;
const TickType_t xDelay = 15000 / portTICK_PERIOD_MS;

void handler_setup(){
    xTaskCreate(&sensor_reading, "ReadingSensors", 8192, NULL, 1, NULL);
    xTaskCreate(&eventHandlerTask, "EventHandler", 8192, NULL, 1, NULL); 
    xTaskCreate(&keeping_connection, "MQTTKeeper", 8192, NULL, 1, NULL);
    xTaskCreate(&wifiMonitoringTask, "WiFi Monitor", 4096, NULL, 1, NULL);
    xEventQueue = xQueueCreate(10, sizeof(SystemEvent_t));
    Serial.println("Handler is ready.");
}

void activatePumb(){
    Serial.println("Pumb Activated");
    digitalWrite(RELAY_PIN, HIGH);
    vTaskDelay( xDelay );
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Pumb Deactivated");

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
                default:
                    Serial.println("Unusual event occured!");
                    break;
            }
        }
    }
}