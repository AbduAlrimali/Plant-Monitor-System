#pragma once
#ifndef HANDLER_H
#define HANDLER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// define events types
typedef enum {
    EVENT_ACTIVATE_PUMB=1,
    EVENT_DATA_READY_FOR_HIVE_MQ=200,
    LCD_SHOW_HUMIDETY=14,
    LCD_SHOW_TEMPRATURE=15,
    LCD_SHOW_MOISTURE=16,
    LCD_SHOW_GAS=17,
    LCD_SHOW_LIGHT=18,
    LCD_SHOW_WATER=19,
    EVENT_WARNING,
    EVENT_NORMAL
} SystemEvent_t;

void handler_setup();
void eventHandlerTask(void* pvParameters);
bool sendSystemEvent(SystemEvent_t event);
void sensor_reading(void* pvParameters);
void sendData();

#endif