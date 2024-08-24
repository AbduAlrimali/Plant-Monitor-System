#include <handler_setup.h>

bool sendSystemEvent(SystemEvent_t event) {
    BaseType_t xStatus = xQueueSendToBack(xEventQueue, &event, (TickType_t) 10); 
    if (xStatus != pdPASS) {
        return false;
    }
    return true;
}

void eventHandlerTask(void* pvParameters){
  SystemEvent_t receivedEvent;
  while (1) {
        if (xQueueReceive(xEventQueue, &receivedEvent, portMAX_DELAY) == pdTRUE) {
            switch (receivedEvent) {
                case EVENT_ACTIVATE_PUMB:
                    activate_pumb();
                    break;
                case EVENT_DATA_READY_FOR_HIVE_MQ:
                    send_data();
                    break;
                default:
                    // Handle unknown event (optional)
                    break;
            }
        }
    }
}