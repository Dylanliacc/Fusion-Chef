#include "weather_service.h"
#include "../network/wifi_manager.h"

void WeatherService::startWeatherTask() {
    xTaskCreate(
        weatherTask,
        "WeatherTask",
        8192,
        NULL,
        1,
        NULL
    );
}

void WeatherService::weatherTask(void * parameter) {
    WeatherService& instance = getInstance();
    
    while(true) {
        Serial.println("\n[Weather] Attempting to fetch weather data...");
        
        if(!WifiManager::getInstance().isConnected()) {
            Serial.println("[Weather] Error: WiFi not connected");
        } else {
            HTTPClient http;
            String url = String("http://") + instance.OPENWEATHER_HOST + "/data/2.5/weather?q=" + 
                        instance.CITY + "&appid=" + instance.OPENWEATHER_API_KEY + "&units=metric";
            
            Serial.println("[Weather] Connecting to OpenWeather API...");
            http.begin(url);
            int httpCode = http.GET();
            
            Serial.printf("[Weather] HTTP Response code: %d\n", httpCode);
            
            if(httpCode > 0) {
                if(httpCode == HTTP_CODE_OK) {
                    String payload = http.getString();
                    Serial.println("[Weather] Data received successfully");
                    
                    StaticJsonDocument<1024> doc;
                    DeserializationError error = deserializeJson(doc, payload);
                    
                    if (!error) {
                        float temp = doc["main"]["temp"];
                        float humidity = doc["main"]["humidity"];
                        const char* description = doc["weather"][0]["description"];
                        
                        Serial.println("[Weather] Data parsed successfully:");
                        Serial.printf("[Weather] Temperature: %.1f°C\n", temp);
                        Serial.printf("[Weather] Humidity: %.1f%%\n", humidity);
                        Serial.printf("[Weather] Description: %s\n", description);
                    } else {
                        Serial.printf("[Weather] JSON parsing failed: %s\n", error.c_str());
                    }
                } else {
                    Serial.printf("[Weather] HTTP Error: %s\n", http.errorToString(httpCode).c_str());
                }
            } else {
                Serial.printf("[Weather] Failed to connect to server. Error: %s\n", http.errorToString(httpCode).c_str());
            }
            http.end();
        }
        Serial.printf("[Weather] Next update in %d seconds\n", instance.WEATHER_UPDATE_INTERVAL/1000);
        vTaskDelay(instance.WEATHER_UPDATE_INTERVAL / portTICK_PERIOD_MS);
    }
}