#include "input.h"
#include<handler.h>
#include<actuators.h>

#define ROWS  4
#define COLS  4

double sensorsData[SENSORS_NUM];
String sensorsName[SENSORS_NUM] = {"Soil Moisture", "Gas", "Light Intensity", "Water Level", "Humidity", "Temperature"};
String unit[SENSORS_NUM] = {"%", "ppm", "lux", "%", "%", "'C"};

uint8_t rowPins[ROWS] = {13, 12, 14, 27}; 
uint8_t colPins[COLS] = {26, 25, 33, 32}; 

char keyMap[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};
Keypad keypad = Keypad(makeKeymap(keyMap), rowPins, colPins, ROWS, COLS );
DHT dht(DHT_PIN, DHT11);

void input_setup(){
  dht.begin();
}

void handleKeypadEvent(void* pvParameters) {
  while(1){
    char key = keypad.getKey();
    if (key) {  // Check if a key was pressed
      Serial.println(key);
      SystemEvent_t keyEvent = static_cast<SystemEvent_t>(key - '0'); // Convert char to enum
      sendSystemEvent(keyEvent);
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

float readTemperature() { //reading from DHT11
    float val = dht.readTemperature();
    if(isnan(val)){
      return 30;
    }
    return val;
}

float readHumidity() { //reading from DHT11
    float val = dht.readHumidity();
    if(isnan(val)){
      return 50;
    }
    return val;
}

int readSoilMoisture() { //reading from soil moisture sensor
    int data= analogRead(SOIL_PIN);
    int w=4095-data;
    return map(w,0,4095,0,100);
}

int readGas() {
    int data = analogRead(GAS_PIN); //reading from gas sensor
    int ppm=map(data,0,4095,20,1000);
    return ppm;
}

int readLightIntensity() {
    int   ldrRawData;
    float ldrLux;

    ldrRawData = analogRead(LIGHT_PIN);
    ldrLux=map(ldrRawData ,0,4095,10,10000);
    return ldrLux;
}

float readWater() { // calculating distance using the ultrasonic sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    int duration = pulseIn(ECHO_PIN, HIGH);
    double distance = (duration * 0.034) / 2; //full = 5, empty = 14
    distance -= 5;
    return min(max(((9-distance)/9.0)*100, 0.0), 100.0);
}

void printData(){
  Serial.print("Temprature: ");
  Serial.println(sensorsData[SENSOR_TEMPERATURE]);
  Serial.print("Humidity: ");
  Serial.println(sensorsData[SENSOR_HUMIDITY]);
  Serial.print("Air Quality: ");
  Serial.println(sensorsData[SENSOR_GAS]);
  Serial.print("Light: ");
  Serial.println(sensorsData[SENSOR_LIGHT]);
  Serial.print("Moisture: ");
  Serial.println(sensorsData[SENSOR_SOIL_MOISTURE]);
  Serial.print("Water level: ");
  Serial.println(sensorsData[SENSOR_WATER]);
}

void sensor_reading(void* pvParameters){
  while(1) {
    // reading sensors values
    sensorsData[SENSOR_SOIL_MOISTURE] = readSoilMoisture();
    sensorsData[SENSOR_GAS] = readGas();
    sensorsData[SENSOR_LIGHT] = readLightIntensity();
    sensorsData[SENSOR_WATER] = readWater();
    sensorsData[SENSOR_HUMIDITY] = readHumidity();
    sensorsData[SENSOR_TEMPERATURE] = readTemperature();
    Serial.println("Sensors read successfully.");

    printData();

  //take action upon sensor readings
    if(sensorsData[0] < 40) {
      sendSystemEvent(EVENT_ACTIVATE_PUMB);
    }

    if(sensorsData[SENSOR_SOIL_MOISTURE] < 40 || sensorsData[SENSOR_SOIL_MOISTURE] > 80){
      sendSystemEvent(EVENT_WARNING_SOIL_MOISTURE);
    } else if(sensorsData[SENSOR_WATER] <= 20){
      sendSystemEvent(EVENT_WARNING_WATER_LEVEL);
    } else if(sensorsData[SENSOR_TEMPERATURE] < 25 || sensorsData[SENSOR_TEMPERATURE] > 35){
      sendSystemEvent(EVENT_WARNING_TEMPERATURE);
    } else if(sensorsData[SENSOR_LIGHT] < 3000){
      sendSystemEvent(EVENT_WARNING_LIGHT);
    } else if(sensorsData[SENSOR_HUMIDITY] < 40 || sensorsData[SENSOR_HUMIDITY] > 80){
      sendSystemEvent(EVENT_WARNING_HUMIDITY);
    }else if(sensorsData[SENSOR_GAS] > 300) {
      sendSystemEvent(EVENT_WARNING_GAS);  
    } else {
      sendSystemEvent(EVENT_NORMAL);
    }

    //send the data to hive
    sendSystemEvent(EVENT_DATA_READY_FOR_HIVE_MQ);
    //wait 15 seconds for next readings
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

void manualIrrigation(void* pvParameters){
  while(1){
    if(currentPumpState == PUMP_OFF){
      int button = digitalRead(PUSHBUTTON_PIN);
      if(button){
        digitalWrite(RELAY_PIN, HIGH);
      } else{
        digitalWrite(RELAY_PIN, LOW);
      }
    }
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}
