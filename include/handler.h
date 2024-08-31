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
    LCD_SHOW_HUMIDETY=10,
    LCD_SHOW_TEMPRATURE=20,
    LCD_SHOW_MOISTURE=30,
    LCD_SHOW_GAS=40,
    LCD_SHOW_LIGHT=50,
    LCD_SHOW_WATER=60
} SystemEvent_t;

void handler_setup();
void eventHandlerTask(void* pvParameters);
bool sendSystemEvent(SystemEvent_t event);
void sensor_reading(void* pvParameters);

const TickType_t xDelay = 15000 / portTICK_PERIOD_MS;

#endif