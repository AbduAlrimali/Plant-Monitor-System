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
    Serial.println("Humidity lcd printed");
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Humidity: " + String(humidityData));
}
void displayTemprature(){
    Serial.println("Temprature lcd printed");
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Temprature: " + String(temperatureData));
}
void displayWater(){
    Serial.println("Water Size lcd printed");
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Water Size: " + String(waterData));
}
void displayGas(){
    Serial.println("Gas lcd printed");
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Gas: " + String(gasData));
}
void displayMoisture(){
    Serial.println("Moisture lcd printed");
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Moisture: " + String(moistureData));
}
void displayLight(){
    Serial.println("Light Intense lcd printed");
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