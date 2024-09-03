#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void activatePumb(void* pvParameters);
void displayLCD(void* pvParameters);
void lcd_setup();
void activateRed();
void activateGreen();

enum PumpState { PUMP_OFF, PUMP_ON, PUMP_WAITING };
extern PumpState currentPumpState;

extern int currentLCDState;

#endif