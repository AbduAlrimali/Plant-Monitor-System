#ifndef WIFI_H
#define WIFI_H
#include <WiFi.h>
#include <WiFiManager.h>

void start_wifi();
void wifiMonitoringTask(void* pvParameters);
void start_wifimanager();

#endif