#pragma once
#ifndef __MYCODE_Z__
#define __MYCODE_Z__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

extern QueueHandle_t xEventQueue;
typedef enum {
    EVENT_AIR_QUALITY_POOR=1,
    EVENT_DATA_READY_FOR_HIVE_MQ,
    EVENT_ACTIVATE_PUMB,
    EVENT_MANUAL_PUMB
} SystemEvent_t;

void eventHandlerTask(void* pvParameters);
bool sendSystemEvent(SystemEvent_t event);

#endif