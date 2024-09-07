#pragma once
#ifndef HANDLER_H
#define HANDLER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// define events types
typedef enum {
    EVENT_ACTIVATE_PUMB=100,
    EVENT_DATA_READY_FOR_HIVE_MQ=200,
    LCD_SHOW_HUMIDETY=1,
    LCD_SHOW_TEMPRATURE=2,
    LCD_SHOW_MOISTURE=3,
    LCD_SHOW_GAS=4,
    LCD_SHOW_LIGHT=5,
    LCD_SHOW_WATER=6,
    EVENT_WARNING,
    EVENT_NORMAL
} SystemEvent_t;

void handler_setup();
void eventHandlerTask(void* pvParameters);
bool sendSystemEvent(SystemEvent_t event);
void sensor_reading(void* pvParameters);

#endif