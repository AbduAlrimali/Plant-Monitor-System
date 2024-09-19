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
    EVENT_LCD_SHOW_HUMIDETY=2,
    EVENT_LCD_SHOW_TEMPRATURE=3,
    EVENT_LCD_SHOW_MOISTURE=5,
    EVENT_LCD_SHOW_GAS=6,
    EVENT_LCD_SHOW_LIGHT=8,
    EVENT_LCD_SHOW_WATER=9,
    EVENT_LCD_SHOW_ERROR=-13, // #
    EVENT_LCD_SHOW_WELCOME=0, // 0
    EVENT_WARNING_HUMIDITY=20,
    EVENT_WARNING_TEMPERATURE,
    EVENT_WARNING_WATER_LEVEL,
    EVENT_WARNING_SOIL_MOISTURE,
    EVENT_WARNING_LIGHT,
    EVENT_WARNING_GAS,
    EVENT_NORMAL
} SystemEvent_t;

void handler_setup();
void eventHandlerTask(void* pvParameters);
bool sendSystemEvent(SystemEvent_t event);
void sensor_reading(void* pvParameters);
void sendData();

#endif