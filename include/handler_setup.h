#ifndef __MYCODE_Y__
#define __MYCODE_Y__
#include <init.h>
#include "freertos/queue.h"

bool sendSystemEvent(SystemEvent_t event);
void eventHandlerTask(void* pvParameters);
extern QueueHandle_t xEventQueue;

#endif