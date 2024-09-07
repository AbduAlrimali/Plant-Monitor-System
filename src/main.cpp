#include <wifi_setup.h>
#include<hivemq.h>
#include <handler.h>
#include <input.h>
#include<actuators.h>

using namespace std;

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
  pinMode(PUSHBUTTON_PIN, INPUT_PULLUP);

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