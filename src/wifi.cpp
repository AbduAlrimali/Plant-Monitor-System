#include <wifi_setup.h>
#include <handler.h>

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());

  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void start_wifimanager(){
  // Create a WiFiManager object
  WiFiManager wifiManager;

  // Set a timeout (in milliseconds) for the configuration portal
  wifiManager.setTimeout(180); 
  wifiManager.setConfigPortalTimeout(180);
  wifiManager.setAPCallback(configModeCallback);

  // Start the WiFiManager configuration portal
  if (!wifiManager.autoConnect("ESP32 Test", "12345678")) { // 192.168.4.1
    Serial.println("Failed to connect and hit timeout");
    // Reset the ESP32 if connection fails
    ESP.restart();
  }

  Serial.println("Connected to WiFi :)");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
static bool wifiConnected = false;  // wifi status to know the previous status