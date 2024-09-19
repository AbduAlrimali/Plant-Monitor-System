#include <wifi_setup.h>
#include<hivemq.h>
#include <handler.h>
#include <input.h>
#include<actuators.h>
#include <ArduinoJson.h>

using namespace std;

void sendData(){
    JsonDocument doc;
    doc["tmp"] = (int) sensorsData[SENSOR_TEMPERATURE];
    doc["distance"] = (int) sensorsData[SENSOR_WATER];
    doc["air"] = (int) sensorsData[SENSOR_GAS];
    doc["light"] = (int) sensorsData[SENSOR_LIGHT];
    doc["humidity"] = (int) sensorsData[SENSOR_HUMIDITY];
    doc["soil moisture"] = (int) sensorsData[SENSOR_SOIL_MOISTURE];

  //convert json to c string
  String output;
  serializeJson(doc, output);
  
  //send to hivemq
  send_hive(output.c_str());
}

void setup() {
  Serial.begin(115200); //UART

  // setup pins
  pinMode(SOIL_PIN, INPUT);
  pinMode(GAS_PIN, INPUT);
  pinMode(LIGHT_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(DHT_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(PUSHBUTTON_PIN, INPUT_PULLDOWN);
  pinMode(BUZZER_PIN, OUTPUT);

  //initialize the components
  input_setup();
  lcd_setup();
  start_wifimanager();
  hivemq_setup();
  handler_setup();
}

void loop() {
  vTaskDelete(NULL);
}