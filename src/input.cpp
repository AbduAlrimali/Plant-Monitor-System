#include "input.h"
#include<handler.h>
#include <ArduinoJson.h>
#include <hivemq.h>
#include<actuators.h>

#define ROWS  4
#define COLS  4

double sensorsData[SENSORS_NUM];

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
      SystemEvent_t keyEvent = static_cast<SystemEvent_t>(key - '#'); // Convert char to enum
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
    int data=4095-analogRead(4);
    return map(data,0,4095,0,170);
}

int readGas() {
    int data = analogRead(GAS_PIN); //reading from gas sensor
    int ppm=map(data,0,4095,200,10000);
    return ppm;
}

int readLightIntensity() {
    int   ldrRawData;
    float ldrLux;

    ldrRawData = analogRead(LIGHT_PIN);
    ldrLux=map(ldrRawData ,500,4095,10,10000);
    return ldrLux;
}

float readWater() { // calculating distance using the ultrasonic sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    int duration = pulseIn(ECHO_PIN, HIGH);
    double distance = (duration * 0.034) / 2; //full = 2, empty = 10
    return max(((8-distance)/8)*100, 0.0);
}

void printData(){
  Serial.print("Temprature: ");
  Serial.println(sensorsData[TEMPERATURE]);
  Serial.print("Humidity: ");
  Serial.println(sensorsData[HUMIDITY]);
  Serial.print("Air Quality: ");
  Serial.println(sensorsData[GAS]);
  Serial.print("Light: ");
  Serial.println(sensorsData[LIGHT]);
  Serial.print("Moisture: ");
  Serial.println(sensorsData[SOIL_MOISTURE]);
  Serial.print("Water level: ");
  Serial.println(sensorsData[WATER]);
}

void sendData(){
    JsonDocument doc;
    doc["tmp"] = (int) sensorsData[TEMPERATURE];
    doc["distance"] = (int) sensorsData[WATER];
    doc["air"] = (int) sensorsData[GAS];
    doc["light"] = (int) sensorsData[LIGHT];
    doc["humidity"] = (int) sensorsData[HUMIDITY];
    doc["soil moisture"] = (int) sensorsData[SOIL_MOISTURE];

  //convert json to c string
  String output;
  serializeJson(doc, output);
  
  //send to hivemq
  send_hive(output.c_str());
}
// green - good
// red - something wrong
// buzzer - no water

void sensor_reading(void* pvParameters){
  while(1) {
    // reading sensors values
    sensorsData[SOIL_MOISTURE] = readSoilMoisture();
    sensorsData[GAS] = readGas();
    sensorsData[LIGHT] = readLightIntensity();
    sensorsData[WATER] = readWater();
    sensorsData[HUMIDITY] = readHumidity();
    sensorsData[TEMPERATURE] = readTemperature();
    Serial.println("Sensors read successfully.");

    printData();

  //take action upon sensor readings
    if(sensorsData[0] < 20) {
      sendSystemEvent(EVENT_ACTIVATE_PUMB);
    }

    if((sensorsData[SOIL_MOISTURE] < 20 && sensorsData[SOIL_MOISTURE] > 60) || sensorsData[WATER]<30  || (sensorsData[TEMPERATURE] < 20 && sensorsData[TEMPERATURE] > 35) || (sensorsData[LIGHT]<3000) || (sensorsData[HUMIDITY]<40 && sensorsData[HUMIDITY] > 60)){
      activateRed();
    } else {
      activateGreen();
    }

    //send the data to hive
    sendData();
    //wait 15 seconds for next readings
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}
