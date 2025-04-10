#include "wifi_manager.h"
#include <Preferences.h>

#define WIFI_CONNECT_TIMEOUT 15000  // 15 seconds
#define NVS_NAMESPACE "wifi_creds"

Preferences preferences;

bool WifiManager::init() {
    WiFi.mode(WIFI_AP_STA);
    preferences.begin(NVS_NAMESPACE, false);
    
    // Try to connect with saved credentials first
    if (tryConnectWithSavedCredentials()) {
        preferences.end();
        return true;
    }
    
    // If saved credentials don't work, start SmartConfig
    bool result = startSmartConfig();
    preferences.end();
    return result;
}

bool WifiManager::tryConnectWithSavedCredentials() {
    String ssid = preferences.getString("ssid", "");
    String password = preferences.getString("password", "");
    
    if (ssid.length() > 0) {
        Serial.println("Trying saved WiFi credentials...");
        WiFi.begin(ssid.c_str(), password.c_str());
        
        unsigned long startTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_CONNECT_TIMEOUT) {
            delay(500);
            Serial.print(".");
        }
        
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nConnected with saved credentials!");
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());
            return true;
        }
        
        Serial.println("\nSaved credentials failed.");
    }
    return false;
}

bool WifiManager::startSmartConfig() {
    Serial.println("Starting SmartConfig...");
    WiFi.beginSmartConfig();
    
    while (!WiFi.smartConfigDone()) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nSmartConfig received.");
    Serial.println("Waiting for WiFi");
    
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_CONNECT_TIMEOUT) {
        delay(500);
        Serial.print(".");
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi Connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        
        // Save the new credentials to NVS
        preferences.putString("ssid", WiFi.SSID());
        preferences.putString("password", WiFi.psk());
        
        return true;
    }
    
    Serial.println("\nConnection failed!");
    return false;
}

bool WifiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

IPAddress WifiManager::getLocalIP() {
    return WiFi.localIP();
}