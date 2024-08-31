#include "input.h"
#include<handler.h>
#include <ArduinoJson.h>
#include <hivemq.h>

int moistureData;
int gasData;
int lightData;
float waterData;
float humidityData;
float temperatureData;

#define ROWS  4
#define COLS  4

uint8_t rowPins[ROWS] = {14, 27, 26, 25}; // GIOP14, GIOP27, GIOP26, GIOP25
uint8_t colPins[COLS] = {33, 32, 18, 19}; // GIOP33, GIOP32, GIOP18, GIOP19

char keyMap[ROWS][COLS] = {
  {'10','20','30', '110'},
  {'40','50','60', '120'},
  {'70','80','90', '130'},
  {'160','0','150', '140'}
};

void handleKeypadEvent(char key) {
  if (key) {  // Check if a key was pressed
    SystemEvent_t keyEvent = static_cast<SystemEvent_t>(key - '0'); // Convert char to enum
    sendSystemEvent(keyEvent); 
  }
}
DHT dht(DHT_PIN, DHT11);
void input_setup(){
    Keypad keypad = Keypad(makeKeymap(keyMap), rowPins, colPins, ROWS, COLS );
    keypad.addEventListener(handleKeypadEvent); // Attach the callback function
}

float readTemperature() { //reading from DHT11
    return dht.readTemperature(); 
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
    double distance = (duration * 0.034) / 2;
    return distance;
}

void sendData(){
    JsonDocument doc;
    doc["tmp"] = temperatureData;
    doc["distance"] = waterData;
    doc["air"] = gasData;
    doc["light"] = lightData;
    doc["humidity"] = humidityData;
    doc["soil moisture"] = moistureData;

  //convert json to c string
  String output;
  serializeJson(doc, output);
  
  //send to hivemq
  send_hive(output.c_str());
}

void sensor_reading(void* pvParameters){
  while(1) {
    // reading sensors values
    moistureData = readSoilMoisture();
    gasData = readGas();
    lightData = readLightIntensity();
    waterData = readWater();
    humidityData = readHumidity();
    temperatureData = readTemperature();
    Serial.println("Sensors read successfully.");

  //take action upon sensor readings
    if(moistureData < 20 || temperatureData>35) {
      sendSystemEvent(EVENT_ACTIVATE_PUMB);
    }
    //send the data to hive
    sendData();
    //wait 15 seconds for next readings
    vTaskDelay(15000 / portTICK_PERIOD_MS);
  }
}
