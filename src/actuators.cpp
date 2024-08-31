#include<actuators.h>
#include<input.h>
#include <handler.h>

int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
void lcd_setup(){
    // initialize LCD
    lcd.init();
    // turn on LCD backlight                      
    lcd.backlight();
}

void displayHumidity(){
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Humidity: " + String(humidityData));
}
void displayTemprature(){
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Temprature: " + String(temperatureData));
}
void displayWater(){
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Water Size: " + String(waterData));
}
void displayGas(){
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Gas: " + String(gasData));
}
void displayMoisture(){
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Moisture: " + String(moistureData));
}
void displayLight(){
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Light Intense: " + String(lightData));
}

// activate the pumb when event is received 
void activatePumb(){
    Serial.println("Pumb Activated");
    digitalWrite(RELAY_PIN, HIGH);
    vTaskDelay( xDelay );
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Pumb Deactivated");
}