#include<actuators.h>
#include<input.h>
#include <handler.h>

int lcdColumns = 16;
int lcdRows = 2;

PumpState currentPumpState=PUMP_OFF;
LCDState currentLCDState=SHOW_WELCOME;
WarnState currentWarnState = NORMAL;
sensorState currentLCDSensor;

String errorsLCD[SENSORS_NUM] = {"Humidity", "Gas", "Light", "Soil Moisture", "Temperature", "Water Level"};

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
        if(currentLCDState==SHOW_WELCOME) {
            lcd.print("Welcome to Smart");
            lcd.setCursor(0, 1);
            lcd.print("PLant.");
            lcd.setCursor(0, 0);
        }
        else if (currentLCDState==SHOW_ERRORS){
            if(currentWarnState == NORMAL){
                lcd.print("All is fine.");
            } else {
                lcd.print(errorsLCD[currentWarnState]+" is");
                lcd.setCursor(0, 1);
                lcd.print("out of range.");
                lcd.setCursor(0, 0);
            } 
        }
         else if(currentLCDState==SHOW_SENSORS) {
            formattedData = String(sensorsData[currentLCDSensor], 2);
            lcd.print(sensorsName[currentLCDSensor]+": ");
            lcd.setCursor(0, 1);
            lcd.print(formattedData + " " + unit[currentLCDSensor]);
            lcd.setCursor(0, 0);
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
                if(sensorsData[SENSOR_SOIL_MOISTURE]<=75){
                    Serial.println("Pumb Activated");
                    digitalWrite(RELAY_PIN, HIGH);
                    currentPumpState = PUMP_WAITING; // Move to the next state
                    previousMillis = millis(); // Record the time
                } else {
                    Serial.println("Plant is watered");
                    currentPumpState = PUMP_OFF;
                }
                break;
            case PUMP_WAITING:
                if (millis() - previousMillis >= 4000) {
                    digitalWrite(RELAY_PIN, LOW);
                    Serial.println("Pumb Deactivated");
                    currentPumpState = PUMP_OFF;
                }
                break;

        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}


void notify(void* pvParameters){
    while(1){
        if(currentWarnState!=NORMAL){
            digitalWrite(GREEN_PIN, LOW);
            digitalWrite(RED_PIN, HIGH);
            digitalWrite(BUZZER_PIN, HIGH);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            digitalWrite(RED_PIN, LOW);
            digitalWrite(BUZZER_PIN, LOW);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        } else {
            digitalWrite(RED_PIN, LOW);
            digitalWrite(GREEN_PIN, HIGH);
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}