#include <handler.h>
#include<hivemq.h>
#include <input.h>
#include<actuators.h>
#include <wifi_setup.h>

QueueHandle_t xEventQueue;

void handler_setup(){
    //setup tasks needed
    xTaskCreatePinnedToCore(&sensor_reading, "ReadingSensors", 4096, NULL, 1, NULL, 0); //sensor reading task
    xTaskCreatePinnedToCore(&eventHandlerTask, "EventHandler", 8192, NULL, 1, NULL, 0); //a task to handle incoming events from mqtt
    xTaskCreatePinnedToCore(&keeping_connection, "MQTTKeeper", 8192, NULL, 1, NULL, 0); // keep mqtt connection
    xTaskCreatePinnedToCore(&wifiMonitoringTask, "WiFi Monitor", 4096, NULL, 1, NULL, 0); //monitor wifi status and take action
    xTaskCreatePinnedToCore(&handleKeypadEvent, "Keypad", 4096, NULL, 1, NULL, 1); 
    xTaskCreatePinnedToCore(&displayLCD, "displayLCD", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(&activatePumb, "activatePumb", 4096, NULL, 1, NULL, 1); 
    xTaskCreatePinnedToCore(&manualIrrigation, "ManualMode", 4096, NULL, 1, NULL, 1); 
    xEventQueue = xQueueCreate(10, sizeof(SystemEvent_t));
    Serial.println("Handler is ready.");
}

// a function to send system events to the handler queue
bool sendSystemEvent(SystemEvent_t event) {
    BaseType_t xStatus = xQueueSendToBack(xEventQueue, &event, (TickType_t) 10); 
    if (xStatus != pdPASS) {
        vTaskDelay(pdMS_TO_TICKS(500));
        return false;
    }
    vTaskDelay(pdMS_TO_TICKS(500));
    return true;
}

//takes action upon the received event
void eventHandlerTask(void* pvParameters){
  SystemEvent_t receivedEvent;
  while (1) {
    // take the event from the queue
        if (xQueueReceive(xEventQueue, &receivedEvent, portMAX_DELAY) == pdTRUE) {
            switch (receivedEvent) {
                case EVENT_ACTIVATE_PUMB:
                    currentPumpState = PUMP_ON;
                    break;
                case EVENT_DATA_READY_FOR_HIVE_MQ:
                    sendData();
                    break;
                case LCD_SHOW_WATER:
                    currentLCDState=SENSOR_WATER;
                    break;
                case LCD_SHOW_GAS:
                    currentLCDState=SENSOR_GAS;
                    break;
                case LCD_SHOW_HUMIDETY:
                    currentLCDState=SENSOR_HUMIDITY;
                    break;
                case LCD_SHOW_TEMPRATURE:
                    currentLCDState=SENSOR_TEMPERATURE;
                    break;
                case LCD_SHOW_LIGHT:
                    currentLCDState=SENSOR_LIGHT;
                    break;
                case LCD_SHOW_MOISTURE:
                    currentLCDState=SENSOR_SOIL_MOISTURE;
                    break;
                case EVENT_WARNING:
                    activateRed();
                    break;
                case EVENT_NORMAL:
                    activateGreen();
                    break;
                default:
                    Serial.println("Unusual event occured!");
                    break;
            }
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}