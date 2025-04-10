#include <Arduino.h>

#include "screen_control/screen_manager.h"
#include "network/wifi_manager.h"
#include "weather/weather_service.h"
#include "gesture_control/gesture_handler.h"
GestureHandler gestureHandler;

void setup() {
    Serial.begin(9600);
    while(!Serial) {
        delay(100);
    }
    if (gestureHandler.init()) {
        Serial.println("PAJ7620U2 initialization failed");
    } else {
        Serial.println("PAJ7620U2 initialization success");
    }
    ScreenManager::getInstance().init();

    // Initialize WiFi and Weather services
    WifiManager::getInstance().init();
    WeatherService::getInstance().startWeatherTask();
}

void loop() {
    gestureHandler.handleGestures();
}
