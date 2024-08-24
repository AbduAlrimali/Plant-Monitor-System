#include<DHT.h>
#include <ArduinoJson.h>
#include<hivemq_setup.h>
#include <init.h>
#include <handler_setup.h>

#define DHTTYPE DHT11

using namespace std;

DHT dht(DHTPin, DHTTYPE);

void setup() {
  Serial.begin(115200); //uart
  pinMode(soilPin, INPUT);
  pinMode(airPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(DHTPin, INPUT);
  hivemq_setup();
  
  xTaskCreate(&sensor_reading, "ReadingSesnors", 5000, NULL, 1, NULL);
  xTaskCreate(&eventHandlerTask, "EventHandler", 1024, NULL, 2, NULL); // Adjust stack size and priority as needed
  xEventQueue = xQueueCreate(10, sizeof(SystemEvent_t));  // Adjust queue size as needed
}


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
  digitalWrite(relayPin, HIGH);
  if(_moisture>=80) {
    digitalWrite(relayPin, LOW);
  }
}

void loop() {}