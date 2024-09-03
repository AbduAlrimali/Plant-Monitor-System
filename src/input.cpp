#include "input.h"
#include<handler.h>
#include <ArduinoJson.h>
#include <hivemq.h>

#define ROWS  4
#define COLS  4

double sensorsData[SENSORS_NUM];
String sensorsName[SENSORS_NUM] = {"Soil Moisture: ", "Gas: ", "Light Intensity: ", "Water Level: ", "Humidity: ", "Soil Moisture: "};

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


int readTemperature() { //reading from DHT11
    return dht.readTemperature(); //int
}

float readHumidity() { //reading from DHT11
    return dht.readHumidity();
}

int readSoilMoisture() { //reading from soil moisture sensor
    int data = analogRead(SOIL_PIN);
    return 100 - ((data / 4095.00) * 100); 
}

int readGas() {
    int data = analogRead(GAS_PIN); //reading from gas sensor
    return 100 - ((data / 4095.00) * 100); 
}

int readLightIntensity() {
    int data = analogRead(LIGHT_PIN); // reading from light sensor
    return 100 - ((data / 4095.00) * 100); 
}

float readWater() { // calculating distance using the ultrasonic sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    int duration = pulseIn(ECHO_PIN, HIGH);
    double distance = (duration * 0.034) / 2; //height = 10cm - r = 2cm
    //
    return distance;
}

void printData(){
  Serial.print("Temprature: ");
  Serial.println(sensorsData[5]);
  Serial.print("Humidity: ");
  Serial.println(sensorsData[4]);
  Serial.print("Air Quality: ");
  Serial.println(sensorsData[1]);
  Serial.print("Light: ");
  Serial.println(sensorsData[2]);
  Serial.print("Moisture: ");
  Serial.println(sensorsData[0]);
  Serial.print("Water level: ");
  Serial.println(sensorsData[3]);
}

void sendData(){
    JsonDocument doc;
    doc["tmp"] = sensorsData[5];
    doc["distance"] = sensorsData[3];
    doc["air"] = sensorsData[1];
    doc["light"] = sensorsData[2];
    doc["humidity"] = sensorsData[4];
    doc["soil moisture"] = sensorsData[0];

  //convert json to c string
  String output;
  serializeJson(doc, output);
  
  //send to hivemq
  send_hive(output.c_str());
}

void sensor_reading(void* pvParameters){
  while(1) {
    // reading sensors values
    sensorsData[0] = readSoilMoisture();
    sensorsData[1] = readGas();
    sensorsData[2] = readLightIntensity();
    sensorsData[3] = readWater();
    sensorsData[4] = readHumidity();
    sensorsData[5] = readTemperature();
    Serial.println("Sensors read successfully.");

    printData();

  //take action upon sensor readings
    if(sensorsData[0] < 20) {
      sendSystemEvent(EVENT_ACTIVATE_PUMB);
    }
    //send the data to hive
    sendData();
    //wait 15 seconds for next readings
    vTaskDelay(15000 / portTICK_PERIOD_MS);
  }
}
