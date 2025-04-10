#include "wifi_manager.h"

bool WifiManager::init() {
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();

    Serial.println("Waiting for SmartConfig.");
    while (!WiFi.smartConfigDone()) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nSmartConfig received.");
    Serial.println("Waiting for WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    return true;
}

bool WifiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

IPAddress WifiManager::getLocalIP() {
    return WiFi.localIP();
}