#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void activatePumb(void* pvParameters);
void displayLCD(void* pvParameters);
void lcd_setup();
void notify(void* pvParameters);

enum PumpState { PUMP_OFF, PUMP_ON, PUMP_WAITING };
enum WarnState { NORMAL, WARNING };
extern PumpState currentPumpState;
extern WarnState currentWarnState;

extern int currentLCDState;

#endif