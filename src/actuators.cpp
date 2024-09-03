#include<actuators.h>
#include<input.h>
#include <handler.h>

int lcdColumns = 16;
int lcdRows = 2;
int currentLCDState=-1;
PumpState currentPumpState = PUMP_OFF;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
void lcd_setup(){
    // initialize LCD
    lcd.init();
    // turn on LCD backlight                      
    lcd.backlight();
}

String formattedData;
void displayLCD(void* pvParameters){
    while(1){
        lcd.setCursor(0, 0);
        lcd.clear();
        if(currentLCDState==-1) {
            lcd.print("Welcome to Smart PLant.");
        } else {
            formattedData = String(sensorsData[currentLCDState], 2);
            lcd.print(sensorsName[currentLCDState] + formattedData);
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
unsigned long previousMillis;
// activate the pumb when event is received 
void activatePumb(void* pvParameters){
    while(1){
        switch (currentPumpState) {
            case PUMP_ON:
                Serial.println("Pumb Activated");
                digitalWrite(RELAY_PIN, HIGH);
                currentPumpState = PUMP_WAITING; // Move to the next state
                previousMillis = millis(); // Record the time
                break;
            case PUMP_WAITING:
                if (millis() - previousMillis >= 15000) {
                    digitalWrite(RELAY_PIN, LOW);
                    Serial.println("Pumb Deactivated");
                    currentPumpState = PUMP_OFF;
                }
                break;
                
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}