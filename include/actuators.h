#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void activatePumb(void* pvParameters);
void displayLCD(void* pvParameters);
void lcd_setup();
void notify(void* pvParameters);

enum PumpState { PUMP_OFF, PUMP_ON, PUMP_WAITING };
enum WarnState { NORMAL=100, WARNING_HUMIDITY=0, WARNING_GAS, WARNING_LIGHT, WARNING_SOIL_MOISTURE, WARNING_TEMPERATURE, WARNING_WATER_LEVEL  };
enum sensorState {LCD_SHOW_HUMIDETY, LCD_SHOW_GAS, LCD_SHOW_LIGHT, LCD_SHOW_SOIL_MOISTURE,LCD_SHOW_TEMPERATURE, LCD_SHOW_WATER_LEVEL };
enum LCDState  {SHOW_WELCOME, SHOW_SENSORS, SHOW_ERRORS};

extern PumpState currentPumpState;
extern LCDState currentLCDState;
extern WarnState currentWarnState;
extern sensorState currentLCDSensor;

#endif