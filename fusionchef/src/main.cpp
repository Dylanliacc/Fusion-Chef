#include <Arduino.h>
#include "gesture_control/gesture_handler.h"
#include "screen_control/screen_manager.h"

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
}

void loop() {
    gestureHandler.handleGestures();
}
