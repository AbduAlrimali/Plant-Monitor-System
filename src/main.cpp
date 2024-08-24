#include<DHT.h>
#include <ArduinoJson.h>
#include<hivemq_setup.h>
#include <handler_setup.h>
#include <init.h>

#define DHTTYPE DHT11
using namespace std;
DHT dht(DHTPin, DHTTYPE);

int soilData;
int _moisture;
int gasData;
int lightData;
float distanceData;
float humidity;
float temperature;

int soilPin=4;   // The ESP32 pin GPIO36 that reads the value from the water sensor
int airPin=34;
int lightPin=35;
int trigPin = 5;    // Trigger
int echoPin = 18;    // Echo
int DHTPin = 15;
int relayPin = 2;  // The ESP32 pin GPIO13 that connects to the relay to control the pump
int powerPin = 0;  // The ESP32 pin GPIO32 that provides the power to the water sensor

void sensor_reading(void* pvParameters){
  while(1) {
    soilData = analogRead(soilPin);
    _moisture = 100 - ((soilData / 4095.00) * 100); // range []
    gasData = analogRead(airPin); //HIGH or LOW // range[]
    lightData = analogRead(lightPin); //range []
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    int duration = pulseIn(echoPin, HIGH);
    distanceData = (duration * 0.034) / 2; // range []
    humidity = dht.readHumidity(); // range []        
    temperature = dht.readTemperature();  // range []

    sendSystemEvent(EVENT_DATA_READY_FOR_HIVE_MQ);
    if(_moisture < 50 || temperature>35){
      sendSystemEvent(EVENT_ACTIVATE_PUMB);
    }
    vTaskDelay(15000 / portTICK_PERIOD_MS);
  }
}


void send_data(){
  JsonDocument doc;
  doc["tmp"] = temperature;
  doc["distance"] = distanceData;
  doc["air"] = gasData;
  doc["light"] = lightData;
  doc["humidity"] = humidity;
  doc["soil moisture"] = _moisture;

  //convert json to c string
  String output;
  serializeJson(doc, output);
  
  //send to hivemq
  send_hive(output.c_str());
}

void activate_pumb(){
  while(_moisture<80){
    digitalWrite(relayPin, HIGH);
  }
  digitalWrite(relayPin, LOW);
}

void setup() {
  Serial.begin(115200); //uart

  pinMode(soilPin, INPUT);
  pinMode(airPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(DHTPin, INPUT);
  hivemq_setup();
  
  xTaskCreate(&sensor_reading, "ReadingSesnors", 8192, NULL, 1, NULL);
  xTaskCreate(&eventHandlerTask, "EventHandler", 8192, NULL, 2, NULL); // Adjust stack size and priority as needed
  xEventQueue = xQueueCreate(10, sizeof(SystemEvent_t));  // Adjust queue size as needed
}

void loop() {}