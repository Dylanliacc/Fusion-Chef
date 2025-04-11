#include <Arduino.h>

#include "screen_control/screen_manager.h"
#include "network/wifi_manager.h"
#include "weather/weather_service.h"
#include "gesture_control/gesture_handler.h"
#include "../test/long_text_test.cpp"
GestureHandler gestureHandler;

void setup() {
    Serial.begin(115200);
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

    // Test long text display
    testLongTextDisplay();
}

void loop() {
    gestureHandler.handleGestures();
    delay(100);
}
