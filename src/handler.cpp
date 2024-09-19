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
    xTaskCreatePinnedToCore(&handleKeypadEvent, "Keypad", 4096, NULL, 1, NULL, 0); 
    xTaskCreatePinnedToCore(&displayLCD, "displayLCD", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(&activatePumb, "activatePumb", 2048, NULL, 1, NULL, 1); 
    xTaskCreatePinnedToCore(&manualIrrigation, "ManualMode", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(&notify, "Notify", 2048, NULL, 1, NULL, 1);
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

                // LCD Sensors
                case EVENT_LCD_SHOW_WATER:
                    currentLCDState=SHOW_SENSORS;
                    currentLCDSensor = LCD_SHOW_WATER_LEVEL;
                    break;
                case EVENT_LCD_SHOW_GAS:
                    currentLCDState=SHOW_SENSORS;
                    currentLCDSensor = LCD_SHOW_GAS;
                    break;
                case EVENT_LCD_SHOW_HUMIDETY:
                    currentLCDState=SHOW_SENSORS;
                    currentLCDSensor = LCD_SHOW_HUMIDETY;
                    break;
                case EVENT_LCD_SHOW_TEMPRATURE:
                    currentLCDState=SHOW_SENSORS;
                    currentLCDSensor = LCD_SHOW_TEMPERATURE;
                    break;
                case EVENT_LCD_SHOW_LIGHT:
                    currentLCDState=SHOW_SENSORS;
                    currentLCDSensor = LCD_SHOW_LIGHT;
                    break;
                case EVENT_LCD_SHOW_MOISTURE:
                    currentLCDState=SHOW_SENSORS;
                    currentLCDSensor = LCD_SHOW_SOIL_MOISTURE;
                    break;
                
                // Navigare LCD States
                case EVENT_LCD_SHOW_WELCOME:
                    currentLCDState=SHOW_WELCOME;
                    break;
                case EVENT_LCD_SHOW_ERROR:
                    currentLCDState=SHOW_ERRORS;
                    break;

                //LCD_Warning
                case EVENT_WARNING_HUMIDITY:
                    currentWarnState = WARNING_HUMIDITY;
                    break;
                case EVENT_WARNING_LIGHT:
                    currentWarnState = WARNING_LIGHT;
                    break;
                case EVENT_WARNING_SOIL_MOISTURE:
                    currentWarnState = WARNING_SOIL_MOISTURE;
                    break;
                case EVENT_WARNING_TEMPERATURE:
                    currentWarnState = WARNING_TEMPERATURE;
                    break;
                case EVENT_WARNING_WATER_LEVEL:
                    currentWarnState = WARNING_WATER_LEVEL;
                    break;
                case EVENT_WARNING_GAS:
                    currentWarnState = WARNING_GAS;
                    break;
                case EVENT_NORMAL:
                    currentWarnState = NORMAL;
                    break;
                default:
                    Serial.println("Unusual event occured!");
                    break;
            }
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}