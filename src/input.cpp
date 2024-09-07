#include "input.h"
#include<handler.h>
#include <ArduinoJson.h>
#include <hivemq.h>
#include<actuators.h>


#define MAX_ADC_READING           4095
#define ADC_REF_VOLTAGE           3.3
#define REF_RESISTANCE            10000    // measure this for best results
#define LUX_CALC_SCALAR           12518931 // from experiment
#define LUX_CALC_EXPONENT         -1.405   // from experiment

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

float readGasPPM(int gasADC) {
    // Constants
    const float Ro = 10.0;  // Example Ro value in kΩ (calibrate this value)
    const int RL = 10;      // Load resistance in kΩ (typically 10kΩ)
    
    // Convert ADC value to voltage
    float voltage = gasADC * (3.3 / 4095.0);  // For ESP32 with a 12-bit ADC and 3.3V reference

    // Calculate the sensor resistance Rs
    float rs = RL * (3.3 - voltage) / voltage;

    // Calculate the ratio Rs/Ro
    float ratio = rs / Ro;

    // Convert the ratio to ppm using a general formula
    // Example formula, needs adjustment based on your calibration and gas type
    float ppm = pow(10, ((log10(ratio) - log10(0.72)) / -0.42));

    return ppm;
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
    int data = analogRead(SOIL_PIN);
    return 100 - ((data / 4095.00) * 100); 
}

int readGas() {
    int data = analogRead(GAS_PIN); //reading from gas sensor
    return readGasPPM(data);
    //return 100 - ((data / 4095.00) * 100); 
}

float lux=0.00,ADC_value=0.0048828125,LDR_value;
int readLightIntensity() {
    int   ldrRawData;
    float resistorVoltage, ldrVoltage;
    float ldrResistance;
    float ldrLux;

    ldrRawData = analogRead(LIGHT_PIN);
    resistorVoltage = (float)ldrRawData * (ADC_REF_VOLTAGE/MAX_ADC_READING)  ;
    // voltage across the LDR is the 5V supply minus the 5k resistor voltage
    ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;
    ldrResistance = REF_RESISTANCE*(ADC_REF_VOLTAGE/ldrVoltage-1);
    ldrLux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);
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

    if((sensorsData[SOIL_MOISTURE] < 20 && sensorsData[SOIL_MOISTURE] > 60) || sensorsData[WATER]<30  || (sensorsData[TEMPERATURE] < 20 && sensorsData[TEMPERATURE] > 35) || (sensorsData[LIGHT]<1000 && sensorsData[LIGHT] > 10000) || (sensorsData[HUMIDITY]<40 && sensorsData[HUMIDITY] > 60)){
      activateRed();
    } else {
      activateGreen();
    }

    //send the data to hive
    sendData();
    //wait 15 seconds for next readings
    vTaskDelay(15000 / portTICK_PERIOD_MS);
  }
}
