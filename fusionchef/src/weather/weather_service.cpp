#include "weather_service.h"
#include "../network/wifi_manager.h"
#include <time.h>

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
            
            http.begin(url);
            int httpCode = http.GET();
            
            if(httpCode > 0) {
                if(httpCode == HTTP_CODE_OK) {
                    String payload = http.getString();
                    
                    StaticJsonDocument<1024> doc;
                    DeserializationError error = deserializeJson(doc, payload);
                    
                    if (!error) {
                        // Get current time for update timestamp
                        time_t now;
                        struct tm timeinfo;
                        time(&now);
                        localtime_r(&now, &timeinfo);
                        char timeStr[9];
                        sprintf(timeStr, "%02d:%02d:%02d", 
                                timeinfo.tm_hour,
                                timeinfo.tm_min,
                                timeinfo.tm_sec);
                        
                        // Update the weather data
                        instance.currentWeather.temperature = doc["main"]["temp"];
                        instance.currentWeather.humidity = doc["main"]["humidity"];
                        instance.currentWeather.description = doc["weather"][0]["description"].as<const char*>();
                        instance.currentWeather.city = instance.CITY;
                        instance.currentWeather.updateTime = String(timeStr);
                        
                        Serial.printf("[Weather] Updated: %s, %.1f°C, %s\n", 
                            instance.currentWeather.city.c_str(),
                            instance.currentWeather.temperature,
                            instance.currentWeather.description.c_str());
                    }
                }
            }
            http.end();
        }
        vTaskDelay(instance.WEATHER_UPDATE_INTERVAL / portTICK_PERIOD_MS);
    }
}