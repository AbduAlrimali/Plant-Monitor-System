#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void lcd_setup();
void displayHumidity();
void displayTemprature();
void displayWater();
void displayGas();
void displayMoisture();
void displayLight();
void activatePumb();

#endif