#ifndef WEATHER_SERVICE_H
#define WEATHER_SERVICE_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

struct WeatherData {
    float temperature;
    float humidity;
    String description;
    String city;
    String updateTime;
};

class WeatherService {
public:
    static WeatherService& getInstance() {
        static WeatherService instance;
        return instance;
    }

    void startWeatherTask();
    WeatherData getLatestWeatherData() { return currentWeather; }
    
private:
    static void weatherTask(void * parameter);
    WeatherService() {}
    WeatherService(const WeatherService&) = delete;
    WeatherService& operator=(const WeatherService&) = delete;

    const char* OPENWEATHER_HOST = "api.openweathermap.org";
    const char* OPENWEATHER_API_KEY = "e67db0cd09f1d1ece4f1259f16516def";
    const char* CITY = "Shenzhen";
    const int WEATHER_UPDATE_INTERVAL = 300000; // 5 minutes
    
    WeatherData currentWeather;
};

#endif // WEATHER_SERVICE_H