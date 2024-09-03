#include "input.h"
#include<handler.h>
#include <ArduinoJson.h>
#include <hivemq.h>

int moistureData;
int gasData;
int lightData;
float waterData;
float humidityData;
int temperatureData;

#define ROWS  4
#define COLS  4

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

    return distance;
}

void printData(){
  Serial.print("Temprature: ");
  Serial.println(temperatureData);
  Serial.print("Humidity: ");
  Serial.println(humidityData);
  Serial.print("Air Quality: ");
  Serial.println(gasData);
  Serial.print("Light: ");
  Serial.println(lightData);
  Serial.print("Moisture: ");
  Serial.println(moistureData);
  Serial.print("Water level: ");
  Serial.println(waterData);
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

    printData();

  //take action upon sensor readings
    if(moistureData < 20) {
      sendSystemEvent(EVENT_ACTIVATE_PUMB);
    }
    //send the data to hive
    sendData();
    //wait 15 seconds for next readings
    vTaskDelay(15000 / portTICK_PERIOD_MS);
  }
}
